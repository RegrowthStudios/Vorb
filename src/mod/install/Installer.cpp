#include "Vorb/stdafx.h"
#include "Vorb/mod/install/Installer.h"

#include "Vorb/io/IOManager.h"
#include "Vorb/io/Keg.h"
#include "Vorb/mod/Mod.h"

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
}

bool vmod::install::Installer::preload(const vio::Path& filepath, bool forUpdate/* = false*/) {
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
    vio::Path preloadLocation = vio::Path(forUpdate ? m_updateDir : m_installDir) / modDir;
    if (preloadLocation.isValid()) return false;

    // Move the mod contents to the appropriate directory.
    m_iomanager->rename(filepath, preloadLocation);
}

bool vmod::install::Installer::install(const nString& modName) {

}

bool vmod::install::Installer::uninstall(const nString& modName) {

}
