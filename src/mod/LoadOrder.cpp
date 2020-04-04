#include "Vorb/stdafx.h"
#include "Vorb/mod/LoadOrder.h"

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

void vmod::LoadOrderManager::init(const vio::Path& loadOrderConfigDir) {
    m_ioManager = vio::IOManager(loadOrderConfigDir, true);

    acquireLoadOrders();
}

void vmod::LoadOrderManager::dispose() {
    m_ioManager = vio::IOManager();

    m_currentLoadOrder = {};
    m_loadOrders = {};
}

const vmod::LoadOrderProfile& vmod::LoadOrderManager::getLoadOrder(const nString& name) const {
    for (auto& loadOrder: m_loadOrders.profiles) {
        if (loadOrder.name == name) {
            return loadOrder;
        }
    }
}

const vmod::LoadOrderProfile& vmod::LoadOrderManager::getCurrentLoadOrder() const {
    return m_currentLoadOrder;
}

const vmod::LoadOrderProfiles& vmod::LoadOrderManager::getAllLoadOrders() const {
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
