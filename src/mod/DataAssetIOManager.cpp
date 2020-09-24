#include "stdafx.h"
#include "Vorb/mod/DataAssetIOManager.h"

#include "Vorb/mod/LoadOrder.h"
#include "Vorb/mod/Mod.h"
#include "Vorb/mod/ModEnvironment.h"

vmod::DataAssetIOManager::DataAssetIOManager() :
    m_modEnv(nullptr),
    m_safeMode(true) {
    // Empty.
}

void vmod::DataAssetIOManager::setGlobalModDirectory(const vio::Path& globalModDir) {
    m_globalModDir = globalModDir;
}

void vmod::DataAssetIOManager::setModEnvironment(const ModEnvironmentBase* modEnv) {
    m_modEnv = modEnv;
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

    if (m_modEnv != nullptr) {
        const ModBasePtrs loadOrder = m_modEnv->getActiveMods();

        vio::Path searchPath;

        // Iterate active mods in priority order and 
        for (const ModBase* mod : loadOrder) {
            searchPath = mod->getModDir() / path;

            if (searchPath.isValid()) {
                resultAbsolutePath = searchPath;
                return true;
            }
        }
    }


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

bool vmod::DataAssetIOManager::readNonMergeableAssetToString(const vio::Path& path, OUT nString& data) {
    vio::Path absPath;
    bool isFound = resolvePath(path, absPath);

    if (!isFound) return false;

    setSafeMode(false);
    readFileToString(absPath, data);
    setSafeMode();

    return true;
}

CALLER_DELETE cString vmod::DataAssetIOManager::readNonMergeableAssetToString(const vio::Path& path) {
    vio::Path absPath;
    bool isFound = resolvePath(path, absPath);

    if (!isFound) return nullptr;

    setSafeMode(false);
    cString res = readFileToString(absPath);
    setSafeMode();

    return res;
}

bool vmod::DataAssetIOManager::readNonMergeableAssetToData(const vio::Path& path, OUT std::vector<ui8>& data) {
    vio::Path absPath;
    bool isFound = resolvePath(path, absPath);

    if (!isFound) return false;

    setSafeMode(false);
    readFileToData(absPath, data);
    setSafeMode();

    return true;
}

bool vmod::DataAssetIOManager::readMergeableAssetToString(const vio::Path& path, OUT nString& data) {
    // TODO(Matthew): Implement.
}

CALLER_DELETE cString vmod::DataAssetIOManager::readMergeableAssetToString(const vio::Path& path) {
    // TODO(Matthew): Implement.
}

bool vmod::DataAssetIOManager::readMergeableAssetToData(const vio::Path& path, OUT std::vector<ui8>& data) {
    // TODO(Matthew): Implement.
}

void vmod::DataAssetIOManager::setSafeMode(bool safeMode /*= true*/) {
    m_safeMode = safeMode;
}
