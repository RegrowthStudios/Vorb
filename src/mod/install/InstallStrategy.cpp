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
    return node->data.as<nString>() == Installer::vanillaOwner.data.as<nString>();
}

keg::Node vmod::install::InstallStrategy::getVanillaOwner() {
    return &Installer::vanillaOwner;
}

vio::IOManager* vmod::install::InstallStrategy::getIOManager() {
    return m_installer->m_iomanager;
}

vio::Path vmod::install::InstallStrategy::getInstallDir()   {
    return m_installer->m_installDir;
}

vio::Path vmod::install::InstallStrategy::getUpdateDir()    {
    return m_installer->m_updateDir;
}

vio::Path vmod::install::InstallStrategy::getGlobalModDir() {
    return m_installer->m_globalModDir;
}

vio::Path vmod::install::InstallStrategy::getBackupDir() {
    return m_installer->m_backupDir;
}

vio::Path vmod::install::InstallStrategy::getManifestDir()  {
    return m_installer->m_manifestDir;
}

vmod::install::Installer::EntryData* vmod::install::InstallStrategy::getEntryData() {
    return &m_installer->m_entryData;
}

keg::Node vmod::install::InstallStrategy::loadCurrentManifestData(const vio::Path& pathname) {
    return m_installer->loadCurrentManifestData(pathname);
}

keg::Node vmod::install::InstallStrategy::loadManifestDataOfMod(const nString& modName, const vio::Path& pathname) {
    return m_installer->loadManifestDataOfMod(modName, pathname);
}

bool vmod::install::ReplaceStrategy::prepare(Installer* installer) {
    return InstallStrategy::prepare(installer);
}

bool vmod::install::ReplaceStrategy::install(const nString& modName) {
    vio::Path newResourcePath = getUpdateDir() / Mod::generateModDirName(modName) / filepath;

    // If the proposed new resource doesn't exist, we can leave happy.
    if (!newResourcePath.isFile()) return true;

    // TODO(Matthew): What if resource doesn't already exist as installed? Replace should still succeed...
    // Load the current manifest data so we can put back a resource as needed.
    keg::Node manifest = loadCurrentManifestData(filepath);

    // Manifest should be simply the name of the mod that previously replaced this asset.
    if (keg::getType(manifest) != keg::NodeType::VALUE) return false;

    // Check if the resource is so-far untouched by mods.
    //     If a mod has touched it then we should make sure to
    //     put their resource back in their install directory.
    //     If no mod has touched it, then we can just store the
    //     vanilla asset in some defined vanilla backup.
    vio::Path backupAssetPath;
    if (ownerIsVanilla(manifest)) {
        backupAssetPath = getBackupDir() / filepath;
    } else {
        nString owner = manifest->data.as<nString>();

        backupAssetPath = getInstallDir() / owner / filepath;
    }
    // Make the move of existing resource to its owner's directory.
    // TODO(Matthew): Not sure rename recursively builds directories...
    if (!filepath.rename(backupAssetPath)) return false;

    // Place new resource in-place.
    newResourcePath.rename(filepath);

    // Save the current manifest into this mod's manifest directory.
    saveManifestDataOfMod(modName, filepath, manifest);

    // Update the manifest to point to this mod and save as current manifest.
    manifest->data = modName;
    saveCurrentManifestData(filepath, manifest);

    return true;
}

bool vmod::install::ReplaceStrategy::uninstall(const nString& modName) {
    // Load the manifest data as of before this mod was installed so we can reinstate the old resource.
    keg::Node manifest = loadManifestDataOfMod(modName, filepath);

    // If manifest does not exist we have nothing to do, so let's leave.
    if (manifest == nullptr) return true;

    // TODO(Matthew): Check current manifest! If this mod isn't owner we shouldn't perform uninstall...

    // TODO(Matthew): Follow through with that promise of destroying install dir.
    // TODO(Matthew): Maybe we'd rather put it in a temporary directory, and have some way of rolling back changes.
    // Move the old asset to this mod's install dir for now (we'll clean that up later  on
    // successful uninstall).
    filepath.rename(getInstallDir() / Mod::generateModDirName(modName) / filepath);

    // Manifest should exist and simply the name of the mod that previously replaced this asset before this mod was installed.
    if (keg::getType(manifest) != keg::NodeType::VALUE) return false;

    // Check if the resource is so-far untouched by mods.
    //     If a mod has touched it then we should make sure to
    //     reinstate their resource.
    //     If no mod has touched it, then we can just restore the
    //     vanilla asset.
    vio::Path oldAssetPath;
    if (ownerIsVanilla(manifest)) {
        oldAssetPath = getBackupDir() / filepath;
    } else {
        nString owner = manifest->data.as<nString>();

        oldAssetPath = getInstallDir() / owner / filepath;
    }
    // Make the move of old resource to the active directory.
    // TODO(Matthew): Not convinced rename by default recursively builds directories...
    if (!oldAssetPath.rename(filepath)) return false;

    // If we are reverting to vanilla asset, delete manifest,
    // otherwise, save new one.
    if (ownerIsVanilla(owner)) {
        deleteCurrentManifestData(filepath);
    } else {
        saveCurrentManifestData(filepath, manifest);
    }

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
