#include "Vorb/stdafx.h"
#include "Vorb/mod/ModEnvironment.h"

#include <algorithm>

void vmod::ModEnvironmentBase::init(const vio::Path& modDir, const vio::Path& loadOrderDir) {
    vio::Path modDir_ = modDir;
    if (!modDir_.isAbsolute()) {
        modDir_ = vio::IOManager::getExecutableDirectory() / modDir_;
    }

    vio::Path loadOrderDir_ = loadOrderDir;
    if (!loadOrderDir_.isAbsolute()) {
        loadOrderDir_ = vio::IOManager::getExecutableDirectory() / loadOrderDir_;
    }

    m_ioManager = vio::IOManager(modDir_, true);

    m_dataAssetIOManager.setModEnvironment(this);

    m_modDir = modDir_;

    discoverMods();

    m_loadOrderManager.init(this, loadOrderDir_);
}

void vmod::ModEnvironmentBase::dispose() {
    m_ioManager = vio::IOManager();

    m_loadOrderManager.dispose();
}

void vmod::ModEnvironmentBase::setModDir(const vio::Path& modDir) {
    m_modDir = modDir;
}

bool vmod::ModEnvironmentBase::deactivateCurrentLoadOrder() {
    const LoadOrderProfile* currentLoadOrder = m_loadOrderManager.getCurrentLoadOrderProfile();

    // If we got a nullptr, then nothing to do.
    if (currentLoadOrder == nullptr) return true;

    // Let every mod perform a deactivation procedure as needed.
    for (auto& modName : currentLoadOrder->mods) {
        const ModBase* mod = getActiveMod(modName);

        if (mod != nullptr) mod->shutdown();
    }

    m_loadOrderManager.setCurrentLoadOrderProfile(nullptr);

    // TODO(Matthew): Reset all compiled data assets.

    ModBaseConstPtrs().swap(m_activeMods);

    return true;
}

bool vmod::ModEnvironmentBase::setActiveLoadOrder(const LoadOrderProfile* newLoadOrder) {
    // If no load order was found with the given name, return early.
    if (newLoadOrder == nullptr) return false;

    // Get current load order profile in case we need to roll back.
    const LoadOrderProfile* currentLoadOrder = m_loadOrderManager.getCurrentLoadOrderProfile();

    // Make sure we are working from a clean slate.
    deactivateCurrentLoadOrder();

    // Do a first pass over mods to activate to ensure all are present.
    for (auto& modName : reverse(newLoadOrder->mods)) {
        const ModBase* mod = getMod(modName);

        // If mod is nullptr, then we are missing a mod needed for the new
        // load order, so revert to previously current load order and return.
        if (mod == nullptr) {
            setActiveLoadOrder(currentLoadOrder);
            return false;
        }
    }

    // TODO(Matthew): Build compiled data assets for new load order.

    // Given all mods in the new load order are present, activate them.
    for (auto& modName : reverse(newLoadOrder->mods)) {
        const ModBase* mod = getMod(modName);

        mod->startup();

        m_activeMods.push_back(mod);
    }

    // Swap order of active mods to be in priority order.
    std::reverse(m_activeMods.begin(), m_activeMods.end());

    return true;
}

bool vmod::ModEnvironmentBase::setActiveLoadOrder(const nString& name) {
    const LoadOrderProfile* newLoadOrder = m_loadOrderManager.getLoadOrderProfile(name);

    return setActiveLoadOrder(newLoadOrder);
}

const vmod::ModBase* vmod::ModEnvironmentBase::getMod(const nString& name) const {
    for (auto& mod : m_mods) {
        if (mod->getModMetadata().name == name) {
            return mod;
        }
    }

    return nullptr;
}

const vmod::ModBase* vmod::ModEnvironmentBase::getActiveMod(const nString& name) const {
    for (auto& mod : m_activeMods) {
        if (mod->getModMetadata().name == name) {
            return mod;
        }
    }

    return nullptr;
}

void vmod::ModEnvironmentBase::discoverMods() {
    vio::Directory* dir = new vio::Directory();
    m_modDir.asDirectory(dir);

    dir->forEachEntry([this](Sender dir VORB_UNUSED, const vio::Path& entry) {
        // We're only interested in mods as directories within the installed directory.
        if (!entry.isDirectory()) return;

        vio::Path metadataFilepath = entry / METADATA_FILENAME;
        if (!metadataFilepath.isFile()) return;

        // Grab mod metadata from file.
        cString metadataRaw = m_ioManager.readFileToString(metadataFilepath);
        if (metadataRaw == nullptr) return;

        // Attempt to parse metadata.
        ModMetadata metadata;
        if (keg::parse(&metadata, metadataRaw, "ModMetadata") != keg::Error::NONE) return;

        registerMod(metadata, entry);
    });
}
