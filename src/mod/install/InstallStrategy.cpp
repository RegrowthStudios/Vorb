#include "Vorb/stdafx.h"
#include "Vorb/mod/install/InstallStrategy.h"

#include "Vorb/io/IOManager.h"
#include "Vorb/mod/install/Installer.h"

void vmod::install::InstallStrategy::registerSingleEntryPoint(Installer* installer, const nString& entryPoint) {
    installer->registerSingleEntryPoint(entryPoint);
}

void vmod::install::InstallStrategy::registerMultiEntryPoint(Installer* installer, const nString& entryPoint) {
    installer->registerMultiEntryPoint(entryPoint);
}

bool vmod::install::ReplaceStrategy::prepare(Installer* installer) {
    // Empty.
}

bool vmod::install::ReplaceStrategy::install(Installer* installer) {
    
}

bool vmod::install::ReplaceStrategy::uninstall(Installer* installer) {
    
}

bool vmod::install::ReplaceInDirStrategy::prepare(Installer* installer) {

}

bool vmod::install::ReplaceInDirStrategy::install(Installer* installer) {
    
}

bool vmod::install::ReplaceInDirStrategy::uninstall(Installer* installer) {
    
}

bool vmod::install::MergeYAMLStrategy::prepare(Installer* installer) {
    registerMultiEntryPoint(installer, listEntry);
}

bool vmod::install::MergeYAMLStrategy::install(Installer* installer) {
    
}

bool vmod::install::MergeYAMLStrategy::uninstall(Installer* installer) {
    
}
