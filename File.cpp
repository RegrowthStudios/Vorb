#include "stdafx.h"
#include "File.h"

#include <boost/filesystem/operations.hpp>

#include "FileOps.h"

namespace fs = boost::filesystem;

vio::File::File(const Path& p) :
    m_path(p) {
    // Empty
}

ui64 vorb::io::File::getSize() const {
    if (m_path.isFile()) {
        fs::path path(m_path.getString());
        return fs::file_size(path);
    } else {
        return 0;
    }
}

vio::FileStream vorb::io::File::create(bool binary /*= true*/) {
    FileStream stream;
    if (buildDirectoryTree(m_path, true)) {
        stream.m_handle.reset(new FileStream::Handle);
        FILE* f = fopen(m_path.getCString(), binary ? "wb" : "w");
        stream.m_handle->m_file = f;
        stream.m_fileCached = f;
    }
    return stream;
}

bool vorb::io::File::resize(ui64 l) const {
    if (!m_path.isFile()) return false;
    fs::path p(m_path.getString());
    boost::system::error_code ec;
    fs::resize_file(p, l, ec);
    return ec.value() == 0;
}

vio::FileStream vio::File::open(bool binary /*= true*/) {
    FileStream stream;
    if (m_path.isFile()) {
        stream.m_handle.reset(new FileStream::Handle);
        FILE* f = fopen(m_path.getCString(), binary ? "ab+" : "a+");
        stream.m_handle->m_file = f;
        stream.m_fileCached = f;
    }
    return stream;
}
vio::FileStream vio::File::openReadOnly(bool binary /*= true*/) {
    FileStream stream;
    if (m_path.isFile()) {
        stream.m_handle.reset(new FileStream::Handle);
        FILE* f = fopen(m_path.getCString(), binary ? "rb" : "r");
        stream.m_handle->m_file = f;
        stream.m_fileCached = f;
    }
    return stream;
}