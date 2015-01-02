#include "stdafx.h"
#include "Path.h"

#include <boost/filesystem.hpp>

#include "Directory.h"
#include "File.h"

namespace fs = boost::filesystem;

vio::Path::Path() : Path("") {
    // Empty
}
vio::Path::Path(const cString p) :
m_path(p) {
    // Empty
}
vio::Path::Path(const nString& p) :
m_path(p) {
    // Empty
}

bool vio::Path::isNull() const {
    return m_path.empty() || m_path.length() == 0;
}
bool vorb::io::Path::isNice() const {
    if (isNull()) return false;
    
    fs::path p(m_path);
    for (auto& piece : p) {
        if (!fs::portable_name(piece.string())) return false;
    }
    return true;
}
bool vorb::io::Path::isNiceFile() const {
    if (isNull()) return false;

    fs::path p(m_path);
    auto iter = p.begin();
    while (iter != p.end()) {
        nString piece = iter->string();
        iter++;
        if (iter == p.end()) {
            if (!fs::portable_file_name(piece)) return false;
        } else {
            if (!fs::portable_directory_name(piece)) return false;
        }
    }
    return true;
}
bool vio::Path::isValid() const {
    if (isNull()) return false;
    return fs::exists(fs::path(m_path));
}
bool vio::Path::isFile() const {
    if (isNull()) return false;
    fs::path p(m_path);
    return fs::exists(p) && fs::is_regular_file(p);
}
bool vio::Path::isDirectory() const {
    if (isNull()) return false;
    fs::path p(m_path);
    return fs::exists(p) && fs::is_directory(p);
}
bool vio::Path::isAbsolute() const {
    if (isNull()) return false;
    return fs::path(m_path).is_absolute();
}

nString vio::Path::getLeaf() const {
    if (isNull()) return "";
    return fs::path(m_path).leaf().string();
}

time_t vio::Path::getLastModTime() const {
    if (isNull()) return 0;
    return boost::filesystem::last_write_time(fs::path(m_path));
}

vio::Path& vio::Path::makeAbsolute() {
    if (isNull() || !isNice()) return *this;
    fs::path p(m_path);
    p = fs::absolute(p);
    m_path = p.string();
    return *this;
}
vio::Path& vio::Path::makeCanonical() {
    if (isNull() || !isNice()) return *this;
    fs::path p(m_path);
    p = fs::canonical(fs::absolute(p));
    m_path = p.string();
    return *this;
}

vio::Path& vio::Path::append(const nString& dir) {
    if (isNull()) return *this;
    fs::path p(m_path);
    p += dir;
    m_path = p.string();
    return *this;
}
vio::Path& vio::Path::concatenate(const nString& dir) {
    if (isNull()) return *this;
    fs::path p(m_path);
    p /= dir;
    m_path = p.string();
    return *this;
}
vio::Path& vio::Path::trimEnd() {
    if (isNull()) return *this;
    fs::path p(m_path);
    p = p.parent_path();
    m_path = p.string();
    return *this;
}

bool vio::Path::asDirectory(OUT Directory* dir) const {
    if (isDirectory()) {
        *dir = Directory(*this);
        return true;
    }
    return false;
}
bool vio::Path::asFile(OUT File* f) const {
    if (isNull()) return false;
    *f = File(*this);
    return true;
}
