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

bool vmod::install::ReplaceStrategy::prepare(Installer* installer) {
    return InstallStrategy::prepare(installer);
}

bool vmod::install::ReplaceStrategy::install() {
    
}

bool vmod::install::ReplaceStrategy::uninstall() {
    
}

bool vmod::install::ReplaceInDirStrategy::prepare(Installer* installer) {
    return InstallStrategy::prepare(installer);
}

bool vmod::install::ReplaceInDirStrategy::install() {
    
}

bool vmod::install::ReplaceInDirStrategy::uninstall() {
    
}

bool vmod::install::MergeYAMLStrategy::prepare(Installer* installer) {
    if (!InstallStrategy::prepare(installer)) return false;

    registerMultiEntryPoint(listEntry);

    return true;
}

bool vmod::install::MergeYAMLStrategy::install() {
    
}

bool vmod::install::MergeYAMLStrategy::uninstall() {
    
}
