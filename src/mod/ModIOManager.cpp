#include "Vorb/stdafx.h"
#include "Vorb/mod/ModIOManager.h"

#include "Vorb/io/FileOps.h"
#include "Vorb/mod/DataAssetIOManager.h"
#include "Vorb/utils.h"

vmod::ModIOManager::ModIOManager() :
    m_dataAssetIOManager(nullptr),
    m_modDir("") {
}

void vmod::ModIOManager::setModDirectory(const vio::Path& path) {
    m_modDir = path;
}
void vmod::ModIOManager::setDataAssetIOManager(const DataAssetIOManager* dataAssetIOManager) {
    m_dataAssetIOManager = dataAssetIOManager;
}

bool vmod::ModIOManager::resolvePath(const vio::Path& path, vio::Path& resultAbsolutePath) const {
    // Mods cannot have absolute paths resolved.
    if (path.isAbsolute()) {
        return false;
    }

    // Read-operations should be performed by data asset IO manager, as this is load-order-aware.
    return m_dataAssetIOManager->resolvePath(path, resultAbsolutePath);
}

bool vmod::ModIOManager::assurePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting) const {
    // Guilty until proven innocent
    if (wasExisting != nullptr) *wasExisting = false;

    // Mods cannot have absolute paths assured.
    if (path.isAbsolute()) {
        return false;
    }

    // Try first to find asset through data asset IO manager.
    bool isFound = m_dataAssetIOManager->resolvePath(path, resultAbsolutePath);

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

    if (path.isNice()) {
        // If we have gotten here, a file must be attempted to be made.
        //   Therefore the only base directory permitted is the mod's own directory.
        vio::Path pathToCreate = m_modDir / path;
        pathToCreate.makeAbsolute();

        if (!vio::buildDirectoryTree(pathToCreate, isFile)) return false;
        if (isFile) {
            vio::File f;
            pathToCreate.asFile(&f);
            f.create(true);
        }
        resultAbsolutePath = pathToCreate;
        return true;
    }

    return false;
}
