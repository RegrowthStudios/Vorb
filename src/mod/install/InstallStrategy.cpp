#include "Vorb/stdafx.h"
#include "Vorb/mod/install/InstallStrategy.h"

#include "Vorb/io/IOManager.h"
#include "Vorb/mod/install/Installer.h"

bool vmod::install::InstallStrategy::prepare(Installer* installer) {
    m_installer = installer;

    return m_installer != nullptr;
}

void vmod::install::InstallStrategy::registerSingleEntryPoint(const nString& entryPoint) {
    m_installer->registerSingleEntryPoint(entryPoint);
}

void vmod::install::InstallStrategy::registerMultiEntryPoint(const nString& entryPoint) {
    m_installer->registerMultiEntryPoint(entryPoint);
}

vio::IOManager* vmod::install::InstallStrategy::getIOManager() {
    return m_installer->m_iomanager;
}

const nString& vmod::install::InstallStrategy::getInstallDir()   {
    return m_installer->m_installDir;
}
const nString& vmod::install::InstallStrategy::getUpdateDir()    {
    return m_installer->m_updateDir;
}
const nString& vmod::install::InstallStrategy::getGlobalModDir() {
    return m_installer->m_globalModDir;
}
const nString& vmod::install::InstallStrategy::getManifestDir()  {
    return m_installer->m_manifestDir;
}

vmod::install::Installer::EntryData* vmod::install::InstallStrategy::getEntryData() {
    return &m_installer->m_entryData;
}

bool vmod::install::ReplaceStrategy::prepare(Installer* installer) {
    return InstallStrategy::prepare(installer);
}

bool vmod::install::ReplaceStrategy::install() {
    return false;
}

bool vmod::install::ReplaceStrategy::uninstall() {
    return false;
}

bool vmod::install::ReplaceInDirStrategy::prepare(Installer* installer) {
    return InstallStrategy::prepare(installer);
}

bool vmod::install::ReplaceInDirStrategy::install() {
    return false;
}

bool vmod::install::ReplaceInDirStrategy::uninstall() {
    return false;
}

bool vmod::install::MergeYAMLStrategy::prepare(Installer* installer) {
    if (!InstallStrategy::prepare(installer)) return false;

    registerMultiEntryPoint(listEntry);

    return true;
}

bool vmod::install::MergeYAMLStrategy::install() {
    return false;
}

bool vmod::install::MergeYAMLStrategy::uninstall() {
    return false;
}
