#include "stdafx.h"
#include "io/File.h"

#include <boost/filesystem/operations.hpp>
//#include <sha256/sha256sum.h>
#include <openssl/sha.h>

#include "io/FileOps.h"
#include "io/FileStream.h"

namespace fs = boost::filesystem;

// 16 x 4-wide string for fopen arguments accessed by FileOpenFlags enum
const cString VORB_IO_FILE_OPEN_ARGS = "\0\0\0\0\0\0\0\0r\0\0\0rb\0\0r+\0\0rb+\0\0\0\0\0\0\0\0\0w\0\0\0wb\0\0w+\0\0wb+\0a\0\0\0ab\0\0a+\0\0ab+\0";

vio::File::File(const Path& p) :
    m_path(p) {
    // Empty
}

ui64 vorb::io::File::length() const {
    if (m_path.isFile()) {
        fs::path path(m_path.getString());
        return fs::file_size(path);
    } else {
        return 0;
    }
}

bool vorb::io::File::resize(const ui64& l) const {
    if (!m_path.isFile()) return false;
    fs::path p(m_path.getString());
    boost::system::error_code ec;
    fs::resize_file(p, l, ec);
    return ec.value() == 0;
}

void vorb::io::File::computeSum(vio::SHA256Sum* sum) const {
    vio::FileSeekOffset l = 0;
    ui8* data = nullptr;

    { // Read file data
        vio::FileStream s = openReadOnly(true);
        l = s.length();
        data = new ui8[l];
        l = (vio::FileSeekOffset)s.read(l, 1, data);
        s.close();
    }

    { // Compute sum
        SHA256_CTX context;
        SHA256_Init(&context);
        SHA256_Update(&context, data, (ui32)l);
        SHA256_Final((unsigned char *)sum, &context);
//        memcpy(sum, context.hash, 32);
    }

    delete[] data;
}

vio::FileStream vio::File::open(FileOpenFlags flags) const {
    FileStream stream(*this);

    // If the file must be created, check for a path to it
    if ((flags & FileOpenFlags::CREATE) != FileOpenFlags::NONE) {
        if (!buildDirectoryTree(m_path, true)) return FileStream();
    }

    // Obtain correct creation arguments
    ui8 location = ((ui8)flags) << 2;
    const cString foFlags = VORB_IO_FILE_OPEN_ARGS + location;

    // Try to open the file
    stream.m_handle.reset(new FileStream::Handle);
    FILE* f = fopen(m_path.getCString(), foFlags);
    stream.m_handle->m_file = f;
    stream.m_fileCached = f;

    return stream;
}
vio::FileStream vio::File::open(const bool& binary /*= true*/) const {
    return open(FileOpenFlags::READ_WRITE_EXISTING | (binary ? FileOpenFlags::BINARY : FileOpenFlags::NONE));
}
vio::FileStream vio::File::openReadOnly(const bool& binary /*= true*/) const {
    return open(FileOpenFlags::READ_ONLY_EXISTING | (binary ? FileOpenFlags::BINARY : FileOpenFlags::NONE));
}
vio::FileStream vio::File::create(const bool& binary /*= true*/) const {
    return open(FileOpenFlags::READ_WRITE_CREATE | (binary ? FileOpenFlags::BINARY : FileOpenFlags::NONE));
}
