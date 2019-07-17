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

vio::FileStream vmod::ModIOManager::openFile(const vio::Path& path, const vio::FileOpenFlags& flags) const {
    vio::Path filePath;
    if ((flags & vio::FileOpenFlags::CREATE) != vio::FileOpenFlags::NONE) {
        if (!assurePath(path, filePath, true)) return vio::FileStream();
    } else {
        if (!resolvePath(path, filePath)) return vio::FileStream();
    }

    vio::File file;
    if (!filePath.asFile(&file)) return vio::FileStream();

    return file.open(flags);
}

bool vmod::ModIOManager::readFileToString(const vio::Path& path, nString& data) const {
    vio::FileStream fs = openFile(path, vio::FileOpenFlags::READ_ONLY_EXISTING);
    if (!fs.isOpened()) return false;

    size_t length = (size_t)fs.length();
    data.resize(length + 1);
    if (length > 0) length = fs.read(length, 1, &(data[0]));
    data[length] = 0;
    return true;
}
cString vmod::ModIOManager::readFileToString(const vio::Path& path) const {
    vio::FileStream fs = openFile(path, vio::FileOpenFlags::READ_ONLY_EXISTING);
    if (!fs.isOpened()) return nullptr;

    size_t length = (size_t)fs.length();
    cString data = new char[length + 1];
    if (length > 0) length = fs.read(length, 1, data);
    data[length] = 0;
    return data;
}
bool vmod::ModIOManager::readFileToData(const vio::Path& path, std::vector<ui8>& data) const {
    vio::FileStream fs = openFile(path, vio::FileOpenFlags::READ_ONLY_EXISTING | vio::FileOpenFlags::BINARY);
    if (!fs.isOpened()) return false;

    size_t length = (size_t)fs.length();
    data.resize(length);
    if (length > 0) fs.read(length, 1, &(data[0]));
    return true;
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

bool vmod::ModIOManager::writeStringToFile(const vio::Path& path, const nString& data) const {
    vio::Path fPath = m_modDir / path;
    vio::File f;
    if (!fPath.asFile(&f)) return false;
    vio::FileStream fs = f.open(vio::FileOpenFlags::WRITE_ONLY_APPEND);
    if (!fs.isOpened()) return false;
    fs.write(data.c_str());
    return true;
}

bool vmod::ModIOManager::makeDirectory(const vio::Path& path) const {
    return buildDirectoryTree(m_modDir / path, false);
}

bool vmod::ModIOManager::fileExists(const vio::Path& path) const {
    vio::Path res;
    if (!resolvePath(path, res)) return false;
    return res.isFile();
}
bool vmod::ModIOManager::directoryExists(const vio::Path& path) const {
    vio::Path res;
    if (!resolvePath(path, res)) return false;
    return res.isDirectory();
}

vio::Path vmod::ModIOManager::globalModDir = "";
