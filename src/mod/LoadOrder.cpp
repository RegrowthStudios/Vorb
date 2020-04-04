#include "Vorb/stdafx.h"
#include "Vorb/mod/LoadOrder.h"

KEG_TYPE_DEF(LoadOrderProfileKeg, vmod::LoadOrderProfileKeg, kt) {
    using namespace keg;
    kt.addValue(
        "name",
        Value::basic(
            offsetof(vmod::LoadOrderProfileKeg, name),
            BasicType::STRING
        )
    );
    kt.addValue(
        "createdTimestamp",
        Value::basic(
            offsetof(vmod::LoadOrderProfileKeg, createdTimestamp),
            BasicType::STRING
        )
    );
    kt.addValue(
        "lastModifiedTimestamp",
        Value::basic(
            offsetof(vmod::LoadOrderProfileKeg, lastModifiedTimestamp),
            BasicType::STRING
        )
    );
}

KEG_TYPE_DEF(LoadOrdersKeg, vmod::LoadOrdersKeg, kt) {
    using namespace keg;
    kt.addValue(
        "currentProfile",
        Value::custom(
            offsetof(vmod::LoadOrdersKeg, currentProfile),
            "LoadOrderProfile"
        )
    );
    kt.addValue(
        "profiles",
        Value::array(
            offsetof(vmod::LoadOrdersKeg, profiles),
            Value::custom(0, "LoadOrderProfile")
        )
    );
}

vmod::LoadOrderManager::LoadOrderManager() {
        // Empty.
}

void vmod::LoadOrderManager::init(const vio::Path& loadOrderConfigDir) {
    m_ioManager = vio::IOManager(loadOrderConfigDir, true);

    acquireLoadOrders();
}

void vmod::LoadOrderManager::dispose() {
    m_ioManager = vio::IOManager();

    m_currentLoadOrder = {};
    m_loadOrders = {};
}

bool vmod::LoadOrderManager::addLoadOrderProfile(LoadOrderProfile&& profile) {
    m_loadOrders.profiles.emplace_back(std::forward<LoadOrderProfile>(profile));
}

bool vmod::LoadOrderManager::replaceLoadOrderProfile(LoadOrderProfile&& profile) {
    removeLoadOrderProfile(profile.name);

    m_loadOrders.profiles.emplace_back(std::forward<LoadOrderProfile>(profile));
}

bool vmod::LoadOrderManager::removeLoadOrderProfile(const nString& name) {
    auto it = std::find_if(
        m_loadOrders.profiles.begin(),
        m_loadOrders.profiles.end(),
        [name] (LoadOrderProfile& profile) {
            return profile.name == name;
        }
    );

    m_loadOrders.profiles.erase(it);
}

const vmod::LoadOrderProfile& vmod::LoadOrderManager::getLoadOrderProfile(const nString& name) const {
    for (auto& loadOrder: m_loadOrders.profiles) {
        if (loadOrder.name == name) {
            return loadOrder;
        }
    }
}

const vmod::LoadOrderProfile& vmod::LoadOrderManager::getCurrentLoadOrderProfile() const {
    return m_currentLoadOrder;
}

const vmod::LoadOrderProfiles& vmod::LoadOrderManager::getAllLoadOrderProfiles() const {
    return m_loadOrders.profiles;
}

void vmod::LoadOrderManager::acquireLoadOrders() {
    // Get load order profiles from file.
    cString profilesRaw = m_ioManager.readFileToString(LOAD_ORDER_PROFILES_FILENAME);
    if (profilesRaw == nullptr) return;

    LoadOrdersKeg loadOrdersKeg;
    // Attempt to parse load order profiles.
    if (keg::parse(&loadOrdersKeg, profilesRaw, "LoadOrdersKeg") != keg::Error::NONE) {
        printf("Could not parse load order profiles.");
    }

    // Swap from Keg-compatible structs using Array to using std::vector.
    //     TODO(Matthew): Is there a better way of doing this? Is it worth
    //                    (/possible) to extend Keg to support this up-front?
    auto copyOutOfKeg = [] (LoadOrderProfileKeg src, LoadOrderProfile& dest) {
        dest = {};
        dest.name = src.name;
        dest.createdTimestamp = src.createdTimestamp;
        dest.lastModifiedTimestamp = src.lastModifiedTimestamp;

        for (auto& name : src.mods) {
            dest.mods.push_back(name);
        }
    };

    copyOutOfKeg(loadOrdersKeg.currentProfile, m_currentLoadOrder);

    LoadOrderProfiles().swap(m_loadOrders.profiles);
    m_loadOrders.profiles.resize(loadOrdersKeg.profiles.size());
    for (size_t i = 0; i < loadOrdersKeg.profiles.size(); ++i) {
        copyOutOfKeg(loadOrdersKeg.profiles[i], m_loadOrders.profiles[i]);
    }
    // TODO(Matthew): Here or at mod set-up (that is, ActiveMod instantiation) check validity of current load order against mod folders.
}
