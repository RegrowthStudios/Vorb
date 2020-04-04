#include "Vorb/stdafx.h"
#include "Vorb/mod/ModIOManager.h"

#include "Vorb/io/FileOps.h"
#include "Vorb/utils.h"

vmod::ModIOManager::ModIOManager() :
    m_vanillaAssetDir(""),
    m_modDir("") {
}

void vmod::ModIOManager::setGlobalModDirectory(const vio::Path& path) {
    globalModDir = path;
}
void vmod::ModIOManager::setVanillaAssetDirectory(const vio::Path& path/* = vio::Path("")*/) {
    m_vanillaAssetDir = path;
}
void vmod::ModIOManager::setModDirectory(const vio::Path& path) {
    m_modDir = path;
}

bool vmod::ModIOManager::resolvePath(const vio::Path& path, vio::Path& resultAbsolutePath) const {
    // Mods cannot have absolute paths resolved.
    if (path.isAbsolute()) {
        return false;
    }

    // Search in order of mod's own dir, then global mod dir, then any vanilla asset dir.
    vio::Path pSearch;

    if (m_modDir.isValid()) {
        pSearch = m_modDir / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            return true;
        }
    }

    if (globalModDir.isValid()) {
        pSearch = globalModDir / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            return true;
        }
    }

    if (m_vanillaAssetDir.isValid()) {
        pSearch = m_vanillaAssetDir / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            return true;
        }
    }

    return false;
}

bool vmod::ModIOManager::assurePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting) const {
    // Guilty until proven innocent
    if (wasExisting) *wasExisting = false;

    // Mods cannot have absolute paths assured.
    if (path.isAbsolute()) {
        return false;
    }

    // Search in order
    vio::Path pSearch;

    if (m_modDir.isValid()) {
        pSearch = m_modDir / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            if (wasExisting) *wasExisting = true;
            return true;
        }
    }

    if (globalModDir.isValid()) {
        pSearch = globalModDir / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            if (wasExisting) *wasExisting = true;
            return true;
        }
    }

    if (m_vanillaAssetDir.isValid()) {
        pSearch = m_vanillaAssetDir / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            if (wasExisting) *wasExisting = true;
            return true;
        }
    }

    if (path.isNice()) {
        // If we have gotten here, a file must be attempted to be made.
        //   Therefore the only base directory permitted is the mod's own directory.
        pSearch = m_modDir / path;
        pSearch.makeAbsolute();

        if (!vio::buildDirectoryTree(pSearch, isFile)) return false;
        if (isFile) {
            vio::File f;
            pSearch.asFile(&f);
            f.create(true);
        }
        resultAbsolutePath = pSearch;
        return true;
    }

    return false;
}

vio::Path vmod::ModIOManager::globalModDir = "";
