#include "Vorb/stdafx.h"
#include "Vorb/mod/ModEnvironment.h"

void vmod::ModEnvironmentBase::init(vio::IOManager* ioManager, const vio::Path& stagedModDir, const vio::Path& installedModDir, const vio::Path& loadOrderDir) {
    m_ioManager = ioManager;
    
    m_stagedModDir = stagedModDir;
    m_installedModDir = installedModDir;
    m_loadOrderManager.init(this, loadOrderDir);

    prepareInstalledMods();
    prepareStagedMods();
}

void vmod::ModEnvironmentBase::dispose() {
    m_loadOrderManager.dispose();
}

void vmod::ModEnvironmentBase::setStagedModDir(const vio::Path& stagedModDir) {
    m_stagedModDir = stagedModDir;
}

void vmod::ModEnvironmentBase::setInstalledModDir(const vio::Path& installedModDir) {
    m_installedModDir = installedModDir;
}

bool vmod::ModEnvironmentBase::uninstallCurrentLoadOrder() {
    const LoadOrderProfile* currentLoadOrder = m_loadOrderManager.getCurrentLoadOrderProfile();

    for (auto& modName : currentLoadOrder->mods) {
        const ModBase* mod = getInstalledMod(modName);

        if (mod != nullptr) m_installer.uninstall(mod);
    }
}

bool vmod::ModEnvironmentBase::installLoadOrder(const nString& name) {
    const LoadOrderProfile* newLoadOrder = m_loadOrderManager.getLoadOrderProfile(name);

    auto& actions = m_loadOrderManager.diffActiveLoadOrderWithInactive(*newLoadOrder);

    for (auto& action : actions) {
        if (action.how == Action::UNINSTALL) {
            m_installer.uninstall(action.mod);
        } else if (action.how == Action::INSTALL) {
            m_installer.install(action.mod);
        } else {
            assert(false);
        }
    }
}

const vmod::ModBase* vmod::ModEnvironmentBase::getInstalledMod(const nString& name) const {
    for (auto& mod : m_installedMods) {
        if (mod.getModMetadata().name == name) {
            return &mod;
        }
    }
}

const vmod::ModBase* vmod::ModEnvironmentBase::getStagedMod(const nString& name) const {
    for (auto& mod : m_stagedMods) {
        if (mod.getModMetadata().name == name) {
            return &mod;
        }
    }
}

void vmod::ModEnvironmentBase::prepareInstalledMods() {
    vio::Directory* dir = nullptr;
    m_installedModDir.asDirectory(dir);
    
    dir->forEachEntry([this](vio::Directory dir, const vio::Path& entry) {
        // We're only interested in mods as directories within the installed directory.
        if (!entry.isDirectory()) return;

        vio::Path metadataFilepath = entry / METADATA_FILENAME;
        if (!metadataFilepath.isFile()) return;

        // Grab mod metadata from file.
        cString metadataRaw = m_ioManager->readFileToString(metadataFilepath);
        if (metadataRaw == nullptr) return;

        // Attempt to parse metadata.
        ModMetadata metadata;
        if (keg::parse(&metadata, metadataRaw, "ModMetadata") != keg::Error::NONE) return;

        addInstalledMod(metadata);
    });
}

void vmod::ModEnvironmentBase::prepareStagedMods() {
    vio::Directory* dir = nullptr;
    m_stagedModDir.asDirectory(dir);
    
    dir->forEachEntry([this](vio::Directory dir, const vio::Path& entry) {
        // We're only interested in mods as directories within the staged directory.
        if (!entry.isDirectory()) return;

        vio::Path metadataFilepath = entry / METADATA_FILENAME;
        if (!metadataFilepath.isFile()) return;

        // Grab mod metadata from file.
        cString metadataRaw = m_ioManager->readFileToString(metadataFilepath);
        if (metadataRaw == nullptr) return;

        // Attempt to parse metadata.
        ModMetadata metadata;
        if (keg::parse(&metadata, metadataRaw, "ModMetadata") != keg::Error::NONE) return;

        addStagedMod(metadata);
    });
}
