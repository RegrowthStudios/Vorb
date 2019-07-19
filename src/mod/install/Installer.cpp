#include "Vorb/stdafx.h"
#include "Vorb/mod/install/Installer.h"

#include "Vorb/io/IOManager.h"
#include "Vorb/io/Keg.h"
#include "Vorb/mod/Mod.h"
#include "Vorb/mod/install/InstallStrategy.h"

vmod::install::Installer::Installer() :
    m_iomanager(nullptr) {
    // Empty.
}

void vmod::install::Installer::init(vio::IOManager* iom, const nString& installDir, const nString& updateDir, const nString& globalModDir) {
    m_iomanager    = iom;
    m_installDir   = installDir;
    m_updateDir    = updateDir;
    m_globalModDir = globalModDir;
}

void vmod::install::Installer::dispose() {
    m_iomanager    = nullptr;
    m_installDir   = "";
    m_updateDir    = "";
    m_globalModDir = "";

    InstallStrategies().swap(m_strategies);
    EntryPoints().swap(m_entryPoints);
    EntryData().swap(m_entryData);
}

bool vmod::install::Installer::registerInstallStrategy(InstallStrategy* strategy) {
    if (!strategy->prepare(this)) return false;

    m_strategies.push_back(strategy);
}

bool vmod::install::Installer::preload(const vio::Path& filepath, bool forUpdate/* = false*/, bool force/* = false*/) {
    // Filepath MUST be absolute. No guesses wanted for which directory to look in.
    if (!filepath.isAbsolute()) return false;

    // Check that the given directory has a mod metadata file.
    vio::Path metadataFilepath = filepath / METADATA_FILENAME;
    if (!metadataFilepath.isFile()) return false;

    // Grab mod metadata from file.
    cString metadataRaw = m_iomanager->readFileToString(metadataFilepath);
    if (metadataRaw == nullptr) return false;

    // Attempt to parse metadata.
    ModMetadata metadata;
    if (keg::parse(&metadata, metadataRaw, "ModMetadata") != keg::Error::NONE) return false;

    // Get mod directory name.
    nString modDir = Mod::generateModDirName(metadata.name);

    // Check that the given mod is not already preloaded into install or update dir.
    // If it is, and we are not forcing the preload, then we must fail.
    vio::Path preloadLocation = vio::Path(forUpdate ? m_updateDir : m_installDir) / modDir;
    if (preloadLocation.isValid() && !force) return false;

    // Move the mod contents to the appropriate directory.
    m_iomanager->rename(filepath, preloadLocation);
}

bool vmod::install::Installer::install(const nString& modName) {
    loadEntryData(modName);
}

bool vmod::install::Installer::update(const nString& modName) {

}

bool vmod::install::Installer::uninstall(const nString& modName) {

}

void vmod::install::Installer::registerSingleEntryPoint(const nString& entryPoint) {
    m_entryPoints.emplace_back(std::make_pair(entryPoint, EntryPointKind::SINGLE));
}

void vmod::install::Installer::registerMultiEntryPoint(const nString& entryPoint) {
    m_entryPoints.emplace_back(std::make_pair(entryPoint, EntryPointKind::MULTI));
}

bool vmod::install::Installer::loadEntryData(const nString& modName, bool forUpdate/* = false*/) {
    nString modDirName = Mod::generateModDirName(modName);

    vio::Path entryPointFilepath = vio::Path(forUpdate ? m_updateDir : m_installDir) / modDirName / ENTRY_POINTS_FILENAME;

    // Grab mod metadata from file.
    cString modEntryPoints = m_iomanager->readFileToString(entryPointFilepath);
    if (modEntryPoints == nullptr) return false;

    // TODO(Matthew): Implement.
}
