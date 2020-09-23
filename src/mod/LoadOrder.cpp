#include "Vorb/stdafx.h"
#include "Vorb/mod/LoadOrder.h"

#include "Vorb/mod/ModEnvironment.h"

KEG_TYPE_DEF(LoadOrderProfile, vmod::LoadOrderProfile, kt) {
    using namespace keg;
    kt.addValue(
        "name",
        Value::basic(
            offsetof(vmod::LoadOrderProfile, name),
            BasicType::STRING
        )
    );
    kt.addValue(
        "createdTimestamp",
        Value::basic(
            offsetof(vmod::LoadOrderProfile, createdTimestamp),
            BasicType::STRING
        )
    );
    kt.addValue(
        "lastModifiedTimestamp",
        Value::basic(
            offsetof(vmod::LoadOrderProfile, lastModifiedTimestamp),
            BasicType::STRING
        )
    );
}

KEG_TYPE_DEF(LoadOrders, vmod::LoadOrders, kt) {
    using namespace keg;
    kt.addValue(
        "currentProfile",
        Value::custom(
            offsetof(vmod::LoadOrders, currentProfile),
            "LoadOrderProfile"
        )
    );
    kt.addValue(
        "profiles",
        Value::array(
            offsetof(vmod::LoadOrders, profiles),
            Value::custom(0, "LoadOrderProfile")
        )
    );
}

vmod::LoadOrderManager::LoadOrderManager() {
        // Empty.
}

void vmod::LoadOrderManager::init(const ModEnvironmentBase* modEnv, const vio::Path& loadOrderConfigDir) {
    m_modEnvironment = modEnv;
    m_ioManager = vio::IOManager(loadOrderConfigDir, true);

    acquireLoadOrders();
}

void vmod::LoadOrderManager::dispose() {
    m_ioManager = vio::IOManager();

    m_currentLoadOrder = {};
    m_loadOrders = {};
}

void vmod::LoadOrderManager::addLoadOrderProfile(LoadOrderProfile&& profile) {
    m_loadOrders.profiles.push_back(std::forward<LoadOrderProfile>(profile));
}

bool vmod::LoadOrderManager::replaceLoadOrderProfile(LoadOrderProfile&& profile) {
    bool didReplace = removeLoadOrderProfile(profile.name);

    m_loadOrders.profiles.push_back(std::forward<LoadOrderProfile>(profile));

    return didReplace;
}

bool vmod::LoadOrderManager::removeLoadOrderProfile(const nString& name) {
    bool isFound = false;
    size_t idx;
    for (idx = 0; idx < m_loadOrders.profiles.size(); ++idx){
        if (m_loadOrders.profiles[idx].name == name) {
            isFound = true;
            break;
        }
    }
    
    if (isFound) {
        return m_loadOrders.profiles.erase(idx);
    }

    return false;
}

const vmod::LoadOrderProfile* vmod::LoadOrderManager::getLoadOrderProfile(const nString& name) const {
    for (auto& loadOrder: m_loadOrders.profiles) {
        if (loadOrder.name == name) {
            return &loadOrder;
        }
    }

    return nullptr;
}

const vmod::LoadOrderProfile* vmod::LoadOrderManager::getCurrentLoadOrderProfile() const {
    return &m_currentLoadOrder;
}

const vmod::LoadOrderProfiles& vmod::LoadOrderManager::getAllLoadOrderProfiles() const {
    return m_loadOrders.profiles;
}

void vmod::LoadOrderManager::acquireLoadOrders() {
    // Get load order profiles from file.
    cString profilesRaw = m_ioManager.readFileToString(LOAD_ORDER_PROFILES_FILENAME);
    if (profilesRaw == nullptr) return;

    // Attempt to parse load order profiles.
    if (keg::parse(&m_loadOrders, profilesRaw, "LoadOrders") != keg::Error::NONE) {
        printf("Could not parse load order profiles.");
    }

    // TODO(Matthew): Here or at mod set-up (that is, ActiveMod instantiation) check validity of current load order against mod folders.
}

vmod::ActionsForMods vmod::LoadOrderManager::diffActiveLoadOrderWithInactive(const LoadOrderProfile& target) {
    // TODO(Matthew): For now this is very strict, to not do maximal action up to a given point,
    //      all mods up to that point must match in name and version in both source and target.
    //      We'd like to be able to relax this condition in time.
    ActionsForMods actions;

    size_t i;

    // While the two load orders match we don't need to do anything.
    for (i = 0; i < m_currentLoadOrder.mods.size(); ++i) {
        if (target.mods.size() <= i) break;
        
        const ModBase* sourceMod = m_modEnvironment->getInstalledMod(m_currentLoadOrder.mods[i]);
        const ModBase* targetMod = m_modEnvironment->getStagedMod(target.mods[i]);

        // If source mod isn't found, then we should "uninstall" down to here as regardless
        // we'll need to reinstall this mod in the other load order profile even if it matches.
        if (sourceMod == nullptr) {
            break;
        }

        // TODO(Matthew): Probably do something other than assert here, but
        //      certainly abort install procedure as we have a malformed
        //      or missing mod package. We should already have a check in
        //      place that a to-be-installed load order profile has all
        //      mods present in staged directory.
        assert(targetMod != nullptr);

        if (sourceMod->getModMetadata().name
                    != targetMod->getModMetadata().name
                || sourceMod->getModMetadata().version
                    != targetMod->getModMetadata().version) {
            break;
        }
    }

    // Now that we've found our first difference, we need to uninstall all mods
    // down to that point of difference, that is, in reverse of their load order.
    for (size_t j = m_currentLoadOrder.mods.size() - 1; j >= i; --j) {
        const ModBase* sourceMod = m_modEnvironment->getInstalledMod(m_currentLoadOrder.mods[i]);

        // If source mod isn't found, then we can't uninstall the mod.
        // For now we just roll over this. In situations where this occurs
        // a full game reinstall may be necessitated.
        if (sourceMod == nullptr) {
            continue;
        }

        actions.emplace_back(ActionForMod{
            Action::UNINSTALL,
            sourceMod
        });
    }

    // We can now install each of the target mods starting from the point of
    // difference.
    for (; i < target.mods.size(); ++i) {
        const ModBase* targetMod = m_modEnvironment->getInstalledMod(target.mods[i]);

        // TODO(Matthew): Probably do something other than assert here, but
        //      certainly abort install procedure as we have a malformed
        //      or missing mod package. We should already have a check in
        //      place that a to-be-installed load order profile has all
        //      mods present in staged directory.
        assert(targetMod != nullptr);

        actions.emplace_back(ActionForMod{
            Action::INSTALL,
            targetMod
        });
    }

    return actions;
}
