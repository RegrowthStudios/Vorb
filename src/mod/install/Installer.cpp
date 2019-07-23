#include "Vorb/stdafx.h"
#include "Vorb/mod/install/Installer.h"

#include "Vorb/io/IOManager.h"
#include "Vorb/io/YAMLImpl.h"
#include "Vorb/mod/Mod.h"
#include "Vorb/mod/install/InstallStrategy.h"

vmod::install::Installer::Installer() :
    m_iomanager(nullptr) {
    // Empty.
}

void vmod::install::Installer::init(vio::IOManager* iom, const nString& installDir, const nString& updateDir, const nString& globalModDir, const nString& backupDir, const nString& manifestDir) {
    m_iomanager    = iom;

    m_installDir   = installDir;
    m_updateDir    = updateDir;
    m_globalModDir = globalModDir;
    m_backupDir    = backupDir;
    m_manifestDir  = manifestDir;

    // Set up the necessary keg context to parse the entry file.
    m_kegContext.env = keg::getGlobalEnvironment();
}

void vmod::install::Installer::dispose() {
    m_iomanager    = nullptr;

    m_installDir   = "";
    m_updateDir    = "";
    m_globalModDir = "";
    m_backupDir    = "";
    m_manifestDir  = "";

    m_kegContext.reader.dispose();

    InstallStrategies().swap(m_strategies);
    EntryPoints().swap(m_entryPoints);
    EntryData().swap(m_entryData);
}

bool vmod::install::Installer::registerInstallStrategy(InstallStrategy* strategy) {
    if (!strategy->prepare(this)) return false;

    m_strategies.push_back(strategy);

    return true;
}

bool vmod::install::Installer::preload(const vio::Path& filepath, bool force/* = false*/) {
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
    vio::Path preloadLocation = m_updateDir / modDir;
    if (preloadLocation.isValid() && !force) return false;

    // Move the mod contents to the appropriate directory.
    return m_iomanager->rename(filepath, preloadLocation);
}

bool vmod::install::Installer::install(const nString& modName) {
    // TODO(Matthew): Implement rollback on any fail.

    loadEntryData(modName);

    for (auto& strategy : m_strategies) {
        strategy->install();
    }

    return true;
}

bool vmod::install::Installer::uninstall(const nString& modName VORB_UNUSED) {
    // TODO(Matthew): Implement rollback on any fail.

    for (auto& strategy : m_strategies) {
        strategy->uninstall();
    }

    return true;
}

void vmod::install::Installer::registerSingleEntryPoint(const nString& entryPoint) {
    m_entryPoints.emplace(std::make_pair(entryPoint, EntryPointKind::SINGLE));
}

void vmod::install::Installer::registerMultiEntryPoint(const nString& entryPoint) {
    m_entryPoints.emplace(std::make_pair(entryPoint, EntryPointKind::MULTI));
}

bool vmod::install::Installer::loadEntryData(const nString& modName) {
    nString modDirName = Mod::generateModDirName(modName);

    vio::Path entryPointFilepath = vio::Path(m_updateDir) / modDirName / ENTRY_POINTS_FILENAME;

    // Grab mod metadata from file.
    cString modEntryPoints = m_iomanager->readFileToString(entryPointFilepath);
    if (modEntryPoints == nullptr) return false;

    // Set up the necessary keg context to parse the entry file.
    try {
        m_kegContext.reader.init(modEntryPoints);
    } catch (YAML::ParserException& e) {
        return false;
    }

    // Get first node of the document.
    keg::Node node = m_kegContext.reader.getFirst();

    // If the file isn't a map, it's wrongly formatted.
    if (keg::getType(node) != keg::NodeType::MAP) return false;

    // TODO(Matthew): Make this neater...
    bool success = true;
    auto processEntry = makeFunctor([this, &success](Sender, const nString& key, keg::Node node){
        for (auto& entryPoint : m_entryPoints) {
            if (entryPoint.first == key) {
                if (entryPoint.second == EntryPointKind::SINGLE) {
                    // If it's a single entry point, allow value or sequence (length = 1) node.
                    if (keg::getType(node) == keg::NodeType::VALUE) {
                        nString entryLoc = node->data.as<nString>();

                        m_entryData.emplace(std::make_pair(key, std::vector<nString>(1,entryLoc)));
                    } else if (keg::getType(node) == keg::NodeType::SEQUENCE
                                && node->data.size() == 1) {
                        nString entryLoc = node->data[0].as<nString>();

                        m_entryData.emplace(std::make_pair(key, std::vector<nString>(1,entryLoc)));
                    } else {
                        success = false;
                        break;
                    }
                } else /*if (entryPoint.second == EntryPointKind::MULTI)*/ {
                    // If it's a multi entry point, allow value or sequence (any length) node.
                    if (keg::getType(node) == keg::NodeType::VALUE) {
                        nString entryLoc = node->data.as<nString>();

                        m_entryData.emplace(std::make_pair(key, std::vector<nString>(1, entryLoc)));
                    } else if (keg::getType(node) == keg::NodeType::SEQUENCE) {
                        std::vector<nString> entryLocs = std::vector<nString>(node->data.size());

                        for (const auto& entryLoc : node->data) {
                            entryLocs.emplace_back(entryLoc.as<nString>());
                        }

                        m_entryData.emplace(std::make_pair(key, entryLocs));
                    } else {
                        success = false;
                        break;
                    }
                }
            }
        }
    });
    m_kegContext.reader.forAllInMap(node, &processEntry);

    // If parse failed, clear out the entry data.
    if (!success) {
        EntryData().swap(m_entryData);
    }

    return success;
}

vio::Path vmod::install::Installer::generateManifestFilepath(const nString& modName, const nString& pathname) {
    return vio::Path(m_manifestDir) / Mod::generateModDirName(modName) / vio::Path(pathname).makeNice() / MANIFEST_FILENAME;
}

vio::Path vmod::install::Installer::generateManifestFilepath(const nString& pathname) {
    return generateManifestFilepath("current", pathname);
}

keg::Node vmod::install::Installer::loadCurrentManifestData(const nString& pathname) {
    vio::Path manifestFilepath = generateManifestFilepath(pathname);

    // Check that a manifest file exists for the given resource.
    if (!manifestFilepath.isFile()) return new keg::Node({
        YAML::Load("'%%none'");
    });

    // Load in manifest data.
    cString manifestData = m_iomanager->readFileToString(manifestFilepath);
    if (manifestData == nullptr) return nullptr;

    try {
        m_kegContext.reader.init(manifestData);
    } catch (YAML::ParserException& e) {
        return nullptr;
    }

    // Return first node of the document.
    return m_kegContext.reader.getFirst();
}

keg::Node vmod::install::Installer::loadManifestDataOfMod(const nString& modName, const nString& pathname) {
    vio::Path manifestFilepath = generateManifestFilepath(modName, pathname);

    // Check that a manifest file exists for the given resource.
    if (!manifestFilepath.isFile()) return nullptr;

    // Load in manifest data.
    cString manifestData = m_iomanager->readFileToString(manifestFilepath);
    if (manifestData == nullptr) return nullptr;

    try {
        m_kegContext.reader.init(manifestData);
    } catch (YAML::ParserException& e) {
        return nullptr;
    }

    // Return first node of the document.
    return m_kegContext.reader.getFirst();
}


keg::YAMLNode vmod::install::Installer::vanillaOwner = keg::Node({
    YAML::Load("'%%none");
});
