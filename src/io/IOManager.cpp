#include "Vorb/stdafx.h"
#include "Vorb/io/IOManager.h"

#include "Vorb/io/FileOps.h"
#include "Vorb/utils.h"

vio::FileStream vio::IOManagerBase::openFile(const Path& path, FileOpenFlags flags /*= FileOpenFlags::READ_ONLY_EXISTING*/) const {
    Path filePath;
    if ((flags & FileOpenFlags::CREATE) != FileOpenFlags::NONE) {
        if (!assurePath(path, filePath, true)) return FileStream();
    } else {
        if (!resolvePath(path, filePath)) return FileStream();
    }

    File f;
    if (!filePath.asFile(&f)) return FileStream();

    return f.open(flags);
}

bool vio::IOManagerBase::readFileToString(const Path& path, nString& data) const {
    FileStream fs = openFile(path, FileOpenFlags::READ_ONLY_EXISTING);
    if (!fs.isOpened()) return false;

    size_t length = (size_t)fs.length();
    data.resize(length + 1);
    if (length > 0) length = fs.read(length, 1, &(data[0]));
    data[length] = 0;
    return true;
}
cString vio::IOManagerBase::readFileToString(const Path& path) const {
    FileStream fs = openFile(path, FileOpenFlags::READ_ONLY_EXISTING);
    if (!fs.isOpened()) return nullptr;

    size_t length = (size_t)fs.length();
    cString data = new char[length + 1];
    if (length > 0) length = fs.read(length, 1, data);
    data[length] = 0;
    return data;
}

bool vio::IOManagerBase::readFileToData(const Path& path, std::vector<ui8>& data) const {
    FileStream fs = openFile(path, FileOpenFlags::READ_ONLY_EXISTING | FileOpenFlags::BINARY);
    if (!fs.isOpened()) return false;

    size_t length = (size_t)fs.length();
    data.resize(length);
    if (length > 0) fs.read(length, 1, &(data[0]));
    return true;
}

bool vio::IOManagerBase::writeStringToFile(const Path& path, const nString& data, FileOpenFlags flags /*= FileOpenFlags::WRITE_ONLY_APPEND*/) const {
    if (flags != FileOpenFlags::WRITE_ONLY_APPEND && flags != FileOpenFlags::WRITE_ONLY_CREATE) return false;

    Path filepath;
    if (!resolvePath(path, filepath)) return false;

    File f;
    if (!filepath.asFile(&f)) return false;

    FileStream fs = f.open(flags);
    if (!fs.isOpened()) return false;

    fs.write(data.c_str());

    return true;
}

bool vio::IOManagerBase::makeDirectory(const Path& path) const {
    Path tmp;
    return assurePath(path, tmp, false);
}

bool vio::IOManagerBase::rename(const Path& src, const Path& dest, bool force/* = false*/) {
    Path resSrc, resDest;
    if (force) {
        if (!assurePath(src, resSrc, true)) return false;
        if (!assurePath(dest, resDest, true)) return false;

        resDest.removeAll();
    } else {
        if (!resolvePath(src, resSrc)) return false;
        if (!resolvePath(dest, resDest)) return false;
    }

    return resSrc.rename(resDest);
}

bool vio::IOManagerBase::fileExists(const Path& path) const {
    Path res;
    if (!resolvePath(path, res)) return false;
    return res.isFile();
}

bool vio::IOManagerBase::directoryExists(const Path& path) const {
    Path res;
    if (!resolvePath(path, res)) return false;
    return res.isDirectory();
}

vio::IOManager::IOManager() :
    m_pathSearch("") {
    // Search Directory Defaults To CWD
    setSearchDirectory(m_pathCWD);
}
vio::IOManager::IOManager(const Path& path) :
m_pathSearch(path) {
    // Empty
}

void vio::IOManager::setSearchDirectory(const Path& s) {
    m_pathSearch = s;
}
void vio::IOManager::setCurrentWorkingDirectory(const Path& s) {
    m_pathCWD = s;
}
void vio::IOManager::setExecutableDirectory(const Path& s) {
    m_pathExec = s;
}

void vio::IOManager::getDirectoryEntries(const Path& dirPath, DirectoryEntries& entries) const {
    Directory dir;
    if (dirPath.asDirectory(&dir)) {
        dir.appendEntries(entries);
    }
}

bool vio::IOManager::resolvePath(const Path& path, Path& resultAbsolutePath) const {
    // Special case if the path is already an absolute path
    if (path.isAbsolute()) {
        if (path.isValid()) {
            resultAbsolutePath = path;
            return true;
        } else {
            return false;
        }
    }

    // Search in order
    Path pSearch;

    if (m_pathSearch.isValid()) {
        pSearch = m_pathSearch / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            return true;
        }
    }

    if (m_pathCWD.isValid()) {
        pSearch = m_pathCWD / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            return true;
        }
    }

    if (m_pathExec.isValid()) {
        pSearch = m_pathExec / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            return true;
        }
    }

    return false;
}
bool vio::IOManager::assurePath(const Path& path, OUT Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting) const {
    return assurePath(path, resultAbsolutePath, IOManagerDirectory::SEARCH, isFile, wasExisting);
}
bool vio::IOManager::assurePath(const Path& path, OUT Path& resultAbsolutePath, IOManagerDirectory creationDirectory, bool isFile, OPT bool* wasExisting) const {
    // Guilty until proven innocent
    if (wasExisting) *wasExisting = false;

    // Special case if the path is already an absolute path
    if (path.isAbsolute()) {
        if (path.isValid()) {
            resultAbsolutePath = path;
            if (wasExisting) *wasExisting = true;
            return true;
        } else {
            if (path.isNice()) {
                if (!vio::buildDirectoryTree(path, isFile)) return false;
                if (isFile) {
                    vio::File f;
                    path.asFile(&f);
                    f.create(true);
                }
                resultAbsolutePath = path;
                return true;
            }
            return false;
        }
    }

    // Search in order
    Path pSearch;

    if (m_pathSearch.isValid()) {
        pSearch = m_pathSearch / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            if (wasExisting) *wasExisting = true;
            return true;
        }
    }

    if (m_pathCWD.isValid()) {
        pSearch = m_pathCWD / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            if (wasExisting) *wasExisting = true;
            return true;
        }
    }

    if (m_pathExec.isValid()) {
        pSearch = m_pathExec / path;
        if (pSearch.isValid()) {
            resultAbsolutePath = pSearch;
            if (wasExisting) *wasExisting = true;
            return true;
        }
    }

    if (path.isNice()) {
        switch (creationDirectory) {
        case IOManagerDirectory::SEARCH:
            pSearch = m_pathSearch / path;
            break;
        case IOManagerDirectory::CURRENT_WORKING:
            pSearch = m_pathCWD / path;
            break;
        case IOManagerDirectory::EXECUTABLE:
            pSearch = m_pathExec / path;
            break;
        default:
            return false;
        }
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

vio::Path vio::IOManager::m_pathExec = "";
vio::Path vio::IOManager::m_pathCWD = "";

