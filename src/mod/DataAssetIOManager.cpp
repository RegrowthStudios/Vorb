#include "Vorb/stdafx.h"
#include "Vorb/mod/DataAssetIOManager.h"

#include "Vorb/mod/LoadOrder.h"
#include "Vorb/mod/Mod.h"
#include "Vorb/mod/ModEnvironment.h"

vmod::DataAssetIOManager::DataAssetIOManager() :
    m_modEnv(nullptr),
    m_safeMode(true) {
    // Empty.
}

void vmod::DataAssetIOManager::setGlobalModDirectory(const vio::Path& globalModDir_) {
    if (globalModDir_.isAbsolute()) {
        globalModDir = globalModDir_;
    } else {
        globalModDir = vio::IOManager::getExecutableDirectory() / globalModDir_;
    }
}

void vmod::DataAssetIOManager::setModEnvironment(const ModEnvironmentBase* modEnv) {
    m_modEnv = modEnv;
}

void vmod::DataAssetIOManager::setVanillaDataDir(const vio::Path& vanillaDataDir_) {
    if (vanillaDataDir_.isAbsolute()) {
        vanillaDataDir = vanillaDataDir_;
    } else {
        vanillaDataDir = vio::IOManager::getExecutableDirectory() / vanillaDataDir_;
    }
}

bool vmod::DataAssetIOManager::resolvePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath) const {
    if (path.isAbsolute()) {
        // While is safe mode absolute filepaths cannot be read.
        if (m_safeMode) return false;

        // When out of safe mode, ascertain validity of path and return true
        // if valid.
        if (path.isValid()) {
            resultAbsolutePath = path;
            return true;
        }

        return false;
    }

    vio::Path searchPath;

    // Check for valid path in each mod directory in priority-order.
    if (m_modEnv != nullptr) {
        const ModBasePtrs loadOrder = m_modEnv->getActiveMods();

        // Iterate active mods in priority order and 
        for (const ModBase* mod : loadOrder) {
            searchPath = mod->getModDir() / path;

            if (searchPath.isValid()) {
                resultAbsolutePath = searchPath;
                return true;
            }
        }
    }

    // Check for valid path in vanilla data directory.
    searchPath = vanillaDataDir / path;

    if (searchPath.isValid()) {
        resultAbsolutePath = searchPath;
        return true;
    }

    return false;
}

bool vmod::DataAssetIOManager::assurePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting /*= nullptr*/) const {
    // While in safe mode absolute filepaths are disallowed.
    if (path.isAbsolute() && m_safeMode) {
        return false;
    }

    // Guilty until proven innocent
    if (wasExisting != nullptr) *wasExisting = false;

    // This IO manager is read-only, so assurePath must behave the same as resolvePath.
    bool isFound = resolvePath(path, resultAbsolutePath);

    // If we found something, we need to check if the kind of thing (directory/file)
    // matches expectation, and return the appropriate information.
    if (isFound) {
        if (wasExisting != nullptr) {
            if (isFile == resultAbsolutePath.isFile()) {
                *wasExisting = true;
                return true;
            } else {
                *wasExisting = false;
                return false;
            }
        } else {
            if (isFile == resultAbsolutePath.isFile()) {
                return true;
            } else {
                return false;
            }
        }
    }

    // Since we can't make the file like usual, we fail if we get to this point.
    return false;
}

bool vmod::DataAssetIOManager::readModFileToString(const vio::Path& path, OUT nString& data, const ModBase* mod) {
    vio::Path absPath = mod->getModDir() / path;

    if (!absPath.isValid()) return false;

    setSafeMode(false);
    readFileToString(absPath, data);
    setSafeMode();

    return true;
}

CALLER_DELETE cString vmod::DataAssetIOManager::readModFileToString(const vio::Path& path, const ModBase* mod) {
    vio::Path absPath = mod->getModDir() / path;

    if (!absPath.isValid()) return nullptr;

    setSafeMode(false);
    cString res = readFileToString(absPath);
    setSafeMode();

    return res;
}

bool vmod::DataAssetIOManager::readModFileToData(const vio::Path& path, OUT std::vector<ui8>& data, const ModBase* mod) {
    vio::Path absPath = mod->getModDir() / path;

    if (!absPath.isValid()) return false;

    setSafeMode(false);
    readFileToData(absPath, data);
    setSafeMode();

    return true;
}

bool vmod::DataAssetIOManager::readVanillaFileToString(const vio::Path& path, OUT nString& data) {
    vio::Path absPath = vanillaDataDir / path;

    if (!absPath.isValid()) return false;

    setSafeMode(false);
    readFileToString(absPath, data);
    setSafeMode();

    return true;
}

CALLER_DELETE cString vmod::DataAssetIOManager::readVanillaFileToString(const vio::Path& path) {
    vio::Path absPath = vanillaDataDir / path;

    if (!absPath.isValid()) return nullptr;

    setSafeMode(false);
    cString res = readFileToString(absPath);
    setSafeMode();

    return res;
}

bool vmod::DataAssetIOManager::readVanillaFileToData(const vio::Path& path, OUT std::vector<ui8>& data) {
    vio::Path absPath = vanillaDataDir / path;

    if (!absPath.isValid()) return false;

    setSafeMode(false);
    readFileToData(absPath, data);
    setSafeMode();

    return true;
}

bool vmod::DataAssetIOManager::readEachFileToString(const vio::Path& path, OUT std::vector<nString>& data) {
    // No absolute filepaths here.
    if (path.isAbsolute()) return false;

    data.clear();

    vio::Path searchPath;

    // Check for valid path in each mod directory in priority-order.
    if (m_modEnv != nullptr) {
        const ModBasePtrs loadOrder = m_modEnv->getActiveMods();

        // Iterate active mods in priority order and 
        for (const ModBase* mod : loadOrder) {
            searchPath = mod->getModDir() / path;

            nString res;
            setSafeMode(false);
            if (readFileToString(searchPath, res)) {
                data.emplace_back(std::move(res));
            }
            setSafeMode();
        }
    }

    // Check for valid path in vanilla data directory.
    searchPath = vanillaDataDir / path;

    nString res;
    setSafeMode(false);
    if (readFileToString(searchPath, res)) {
        data.emplace_back(std::move(res));
    }
    setSafeMode();

    return data.size() > 0;
}

std::vector<CALLER_DELETE cString> vmod::DataAssetIOManager::readEachFileToString(const vio::Path& path) {
    // No absolute filepaths here.
    if (path.isAbsolute()) return std::vector<cString>();

    std::vector<cString> buffers;

    vio::Path searchPath;

    // Check for valid path in each mod directory in priority-order.
    if (m_modEnv != nullptr) {
        const ModBasePtrs loadOrder = m_modEnv->getActiveMods();

        // Iterate active mods in priority order and 
        for (const ModBase* mod : loadOrder) {
            searchPath = mod->getModDir() / path;

            setSafeMode(false);
            cString res = readFileToString(searchPath);
            setSafeMode();

            if (res != nullptr) buffers.push_back(res);
        }
    }

    // Check for valid path in vanilla data directory.
    searchPath = vanillaDataDir / path;

    setSafeMode(false);
    cString res = readFileToString(searchPath);
    setSafeMode();

    if (res != nullptr) buffers.push_back(res);

    return buffers;
}

bool vmod::DataAssetIOManager::readEachFileToData(const vio::Path& path, OUT std::vector<std::vector<ui8>>& data) {
    // No absolute filepaths here.
    if (path.isAbsolute()) return false;

    data.clear();

    vio::Path searchPath;

    // Check for valid path in each mod directory in priority-order.
    if (m_modEnv != nullptr) {
        const ModBasePtrs loadOrder = m_modEnv->getActiveMods();

        // Iterate active mods in priority order and 
        for (const ModBase* mod : loadOrder) {
            searchPath = mod->getModDir() / path;

            std::vector<ui8> res;
            setSafeMode(false);
            if (readFileToData(searchPath, res)) {
                data.emplace_back(std::move(res));
            }
            setSafeMode();
        }
    }

    // Check for valid path in vanilla data directory.
    searchPath = vanillaDataDir / path;

    std::vector<ui8> res;
    setSafeMode(false);
    if (readFileToData(searchPath, res)) {
        data.emplace_back(std::move(res));
    }
    setSafeMode();

    return data.size() > 0;
}

void vmod::DataAssetIOManager::setSafeMode(bool safeMode /*= true*/) {
    m_safeMode = safeMode;
}

vio::Path vmod::DataAssetIOManager::vanillaDataDir = "";
vio::Path vmod::DataAssetIOManager::globalModDir = "";
