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
        Value::basic(
            offsetof(vmod::LoadOrders, currentProfile),
            BasicType::STRING
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

vmod::LoadOrderManager::LoadOrderManager() :
    m_modEnvironment(nullptr), m_currentLoadOrder(nullptr) {
        // Empty.
}

void vmod::LoadOrderManager::init(const ModEnvironmentBase* modEnv, const vio::Path& loadOrderConfigDir) {
    m_modEnvironment = modEnv;
    m_ioManager = vio::IOManager(loadOrderConfigDir, true);

    acquireLoadOrders();
}

void vmod::LoadOrderManager::dispose() {
    m_ioManager = vio::IOManager();

    m_loadOrders = {};
}

bool vmod::LoadOrderManager::addLoadOrderProfile(LoadOrderProfile&& profile) {
    if (getLoadOrderProfile(profile.name) == nullptr) {
        return false;
    }

    m_loadOrders.profiles.push_back(std::forward<LoadOrderProfile>(profile));

    return true;
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
    return m_currentLoadOrder;
}

const vmod::LoadOrderProfiles& vmod::LoadOrderManager::getAllLoadOrderProfiles() const {
    return m_loadOrders.profiles;
}

void vmod::LoadOrderManager::setCurrentLoadOrderProfile(const LoadOrderProfile* profile) {
    m_currentLoadOrder = profile;
}

bool vmod::LoadOrderManager::setCurrentLoadOrderProfile(const nString& name) {
    const LoadOrderProfile* loadOrderProfileToMakeCurrent = getLoadOrderProfile(name);

    if (loadOrderProfileToMakeCurrent == nullptr) {
        return false;
    }

    m_currentLoadOrder = loadOrderProfileToMakeCurrent;

    return true;
}

void vmod::LoadOrderManager::acquireLoadOrders() {
    // Get load order profiles from file.
    cString profilesRaw = m_ioManager.readFileToString(LOAD_ORDER_PROFILES_FILENAME);
    if (profilesRaw == nullptr) return;

    // Attempt to parse load order profiles.
    if (keg::parse(&m_loadOrders, profilesRaw, "LoadOrders") != keg::Error::NONE) {
        printf("Could not parse load order profiles.");
    }
}
