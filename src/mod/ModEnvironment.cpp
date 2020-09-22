#include "Vorb/stdafx.h"
#include "Vorb/mod/ModEnvironment.h"

void vmod::ModEnvironmentBase::init(const vio::Path& stagedModDir, const vio::Path& installedModDir, const vio::Path& loadOrderDir) {
    m_stagedModDir = stagedModDir;
    m_installedModDir = installedModDir;
    m_loadOrderManager.init(this, loadOrderDir);

    // TODO(Matthew): Initialise the installer.

    prepareInstalledMods();
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
