#include "Vorb/stdafx.h"
#include "Vorb/mod/ModEnvironment.h"

void vmod::ModEnvironmentBase::init(const vio::Path& globalModDir, const vio::Path& loadOrderDir) {
    m_globalModDir = globalModDir;
    m_loadOrderManager.init(loadOrderDir);

    prepareActiveMods();
}

void vmod::ModEnvironmentBase::dispose() {
    m_loadOrderManager.dispose();
}

void vmod::ModEnvironmentBase::setGlobalModDir(const vio::Path& globalModDir) {
    m_globalModDir = globalModDir;
}


