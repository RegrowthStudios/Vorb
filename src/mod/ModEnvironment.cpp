#include "Vorb/stdafx.h"
#include "Vorb/mod/ModEnvironment.h"

void vmod::ModEnvironmentBase::init(const vio::Path& globalModDir, const vio::Path& loadOrderDir) {
    m_globalModDir = globalModDir;
    m_loadOrderManager.init(this, loadOrderDir);

    // TODO(Matthew): Initialise the installer.

    prepareActiveMods();
}

void vmod::ModEnvironmentBase::dispose() {
    m_loadOrderManager.dispose();
}

void vmod::ModEnvironmentBase::setGlobalModDir(const vio::Path& globalModDir) {
    m_globalModDir = globalModDir;
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
