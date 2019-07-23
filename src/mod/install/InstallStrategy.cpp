#include "Vorb/stdafx.h"
#include "Vorb/mod/install/InstallStrategy.h"

#include "Vorb/io/IOManager.h"
#include "Vorb/io/YAMLImpl.h"
#include "Vorb/mod/Mod.h"
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

const nString& vmod::install::InstallStrategy::getBackupDir() {
    return m_installer->m_backupDir;
}

const nString& vmod::install::InstallStrategy::getManifestDir()  {
    return m_installer->m_manifestDir;
}

vmod::install::Installer::EntryData* vmod::install::InstallStrategy::getEntryData() {
    return &m_installer->m_entryData;
}

keg::Node vmod::install::InstallStrategy::loadCurrentManifestData(const nString& pathname) {
    return m_installer->loadCurrentManifestData(pathname);
}

keg::Node vmod::install::InstallStrategy::loadManifestDataOfMod(const nString& modName, const nString& pathname) {
    return m_installer->loadManifestDataOfMod(modName, pathname);
}

bool vmod::install::ReplaceStrategy::prepare(Installer* installer) {
    return InstallStrategy::prepare(installer);
}

bool vmod::install::ReplaceStrategy::install(const nString& modName) {
    vio::Path newResourcePath = getUpdateDir() / Mod::generateModDirName(modName) / filepath;

    // If the proposed new resource doesn't exist, we  can leave happy.
    if (!newResourcePath.isFile()) return true;

    // Load the current manifest data so we can put back a resource as needed.
    keg::Node manifest = loadCurrentManifestData(filepath);

    // Manifest should be simply the name of the mod that previously 
    if (keg::getType(manifest) != keg::NodeType::VALUE) return false;

    nString owner = manifest->data.as<nString>();

    // Check if the resource is so-far untouched by mods.
    //     If a mod has touched it then we should make sure to
    //     put their resource back in their install directory.
    //     If no mod has touched it, then we can just store the
    //     vanilla asset in some defined vanilla backup.
    vio::Path backupAssetPath;
    if (owner != "%%none") {
        backupAssetPath = getInstallDir() / owner / filepath;
    } else {
        backupAssetPath = getBackupDir() / filepath;
    }
    // TODO(Matthew): Not convinced rename by default recursively builds directories...
    if (!filepath.rename(backupAssetPath)) return false;

    newResourcePath.rename(filepath);

    manifest->data = modName;

    saveCurrentManifestData(filepath, manifest);

    return true;
}

bool vmod::install::ReplaceStrategy::uninstall(const nString& modName) {
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
