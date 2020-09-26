#include "Vorb/stdafx.h"
#include "Vorb/io/Path.h"

#include "Vorb/io/filesystem.h"
#include "Vorb/io/Directory.h"
#include "Vorb/io/File.h"

#ifdef _WIN32
#include <sys/stat.h>
#endif

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

/************************************************************************\
 * Adapted from Boost::Filesystem portability functions.                *
 *                                                                      *
 * Copyright 2002-2005 Beman Dawes                                      *
 *                                                                      *
 * Use, modification, and distribution is subject to the Boost Software *
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy *
 * at http://www.boost.org/LICENSE_1_0.txt)                             *
\************************************************************************/

const nString invalidWindowsChars("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F<>:\"/\\|\0");

const nString validPOSIXChars( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-");

bool isPOSIXFragment(const nString& fragment) {
    return fragment.size() != 0
            && fragment.find_first_not_of(validPOSIXChars) == std::string::npos;
}

bool isWindowsFragment(const nString& fragment)
{
    return fragment.size() != 0
            && fragment[0] != ' '
            && fragment.find_first_of(invalidWindowsChars) == nString::npos
            && *(fragment.end() - 1) != ' '
            && (*(fragment.end() - 1) != '.'
                || fragment.length() == 1
                || fragment == "..");
}

bool isNiceFragment(const nString& fragment) {
    return fragment.size() != 0
            && (fragment == "."
                || fragment == ".."
                || (isWindowsFragment(fragment)
                    && isPOSIXFragment(fragment)
                    && fragment[0] != '.'
                    && fragment[0] != '-'));
}

bool isNiceDirectoryFragment(const nString& fragment)
{
    return fragment == "."
            || fragment == ".."
            || (isNiceFragment(fragment)
                && fragment.find('.') == nString::npos);
}

bool isNiceFileFragment(const nString& fragment)
{
    nString::size_type pos;
    return isNiceFragment(fragment)
            && fragment != "."
            && fragment != ".."
            && ((pos = fragment.find('.')) == std::string::npos
                || (fragment.find('.', pos + 1) == std::string::npos
                && (pos + 5) > fragment.length()));
}

/****************************************************************\
 * End adaptation from Boost::Filesystem portability functions. *
\****************************************************************/

bool vorb::io::Path::isNice() const {
    if (isNull()) return false;
    
    fs::path p(m_path);
    for (auto& piece : p) {
        if (!isNiceFragment(piece.string())) return false;
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
            if (!isNiceFileFragment(piece)) return false;
        } else {
            if (!isNiceDirectoryFragment(piece)) return false;
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
    return fs::path(m_path).filename().string();
}

template<typename Type> time_t convertToTimeT(Type time VORB_MAYBE_UNUSED) {
    // return Type::clock::to_time_t(time);
    return 0;
}

template<> time_t convertToTimeT<time_t>(time_t time) {
    return time;
}

time_t vio::Path::getLastModTime() const {
    if (isNull()) return 0;

#ifdef _WIN32
    fs::path filePath(m_path);

    struct _stat64 fileInfo;
    if(_wstati64(filePath.wstring().c_str(), &fileInfo)!=0)
        return 0;
    return fileInfo.st_mtime;
#else
    auto fsTime=fs::last_write_time(fs::path(m_path));
    return convertToTimeT(fsTime);
#endif
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

size_t std::hash<vio::Path>::operator()(const vio::Path& path) const {
    return std::hash<nString>::operator()(path.getString());
}
