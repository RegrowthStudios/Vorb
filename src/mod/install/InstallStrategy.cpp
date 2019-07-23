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

bool vmod::install::InstallStrategy::ownerIsVanilla(keg::Node node) {
    return m_installer->ownerIsVanilla(node);
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

    // Manifest should be simply the name of the mod that previously replaced this asset.
    if (keg::getType(manifest) != keg::NodeType::VALUE) return false;

    nString owner = manifest->data.as<nString>();

    // Check if the resource is so-far untouched by mods.
    //     If a mod has touched it then we should make sure to
    //     put their resource back in their install directory.
    //     If no mod has touched it, then we can just store the
    //     vanilla asset in some defined vanilla backup.
    vio::Path backupAssetPath;
    if (ownerIsVanilla(owner)) {
        backupAssetPath = getBackupDir() / filepath;
    } else {
        backupAssetPath = getInstallDir() / owner / filepath;
    }
    // TODO(Matthew): Not convinced rename by default recursively builds directories...
    if (!filepath.rename(backupAssetPath)) return false;

    newResourcePath.rename(filepath);

    saveManifestDataOfMod(modName, filepath, manifest);

    manifest->data = modName;

    saveCurrentManifestData(filepath, manifest);

    // TODO(Matthew): Not totally clear on clean-up, but assume this suffices for now...
    delete manifest;

    return true;
}

bool vmod::install::ReplaceStrategy::uninstall(const nString& modName) {
    // Load the manifest data as of before this mod was installed so we can reinstate the old resource.
    keg::Node manifest = loadManifestDataOfMod(modName, filepath);

    // If manifest does not exist we have nothing to do, so let's leave.
    if (manifest == nullptr) return true;

    // TODO(Matthew): Follow through with that promise of destroying install dir.
    // Move the old asset to this mod's install dir for now (we'll clean that up later  on
    // successful uninstall).
    filepath.rename(getInstallDir() / Mod::generateModDirName(modName) / filepath);

    // Manifest should exist and simply the name of the mod that previously replaced this asset before this mod was installed.
    if (keg::getType(manifest) != keg::NodeType::VALUE) return false;

    nString owner = manifest->data.as<nString>();

    // Check if the resource is so-far untouched by mods.
    //     If a mod has touched it then we should make sure to
    //     reinstate their resource.
    //     If no mod has touched it, then we can just restore the
    //     vanilla asset.
    vio::Path oldAssetPath;
    if (ownerIsVanilla(owner)) {
        oldAssetPath = getBackupDir() / filepath;
    } else {
        oldAssetPath = getInstallDir() / owner / filepath;
    }

    // TODO(Matthew): Not convinced rename by default recursively builds directories...
    if (!oldAssetPath.rename(filepath)) return false;

        manifest->data = owner;
    if (manifest )
    saveCurrentManifestData(filepath, manifest);

    // TODO(Matthew): Not totally clear on clean-up, but assume this suffices for now...
    delete manifest;

    return true;
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
