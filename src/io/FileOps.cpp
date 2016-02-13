#include "stdafx.h"
#include "io/FileOps.h"

#include <boost/filesystem.hpp>

#include "os.h"

namespace fs = boost::filesystem;

bool vio::buildDirectoryTree(const Path& path, bool omitEnd /*= false*/) {
    if (path.isNull()) return false;
    
    Path pa = path;
    if (omitEnd) {
        pa.trimEnd();
        if (pa.isNull()) return true; // Code successfully did nothing
    }

    fs::path bp(pa.getString());
    if (fs::exists(bp)) return true;
    return fs::create_directories(bp);
}

#if defined(OS_WINDOWS)
namespace vorb {
    namespace io {
        DWORD CALLBACK windowsCopyProgressRoutine(
            LARGE_INTEGER totalFileSize,
            LARGE_INTEGER totalBytesTransferred,
            LARGE_INTEGER streamSize,
            LARGE_INTEGER streamBytesTransferred,
            DWORD         dwStreamNumber,
            DWORD         dwCallbackReason,
            HANDLE        hSourceFile,
            HANDLE        hDestinationFile,
            LPVOID        lpData
            ) {
            // Only process chunk updates
            if (dwCallbackReason != CALLBACK_CHUNK_FINISHED) return PROGRESS_CONTINUE;

            // Update and determine how to continue the copying operation
            return ((CopyFileProgressCallback*)lpData)->invoke(totalBytesTransferred.QuadPart, totalFileSize.QuadPart) ? PROGRESS_CANCEL : PROGRESS_CONTINUE;
        }
    }
}
bool vio::copyWithProgress(const Path& src, const Path& dest, CopyFileProgressCallback f) {
    BOOL cancelled = 0;
    auto ret = CopyFileEx(src.getCString(), dest.getCString(), windowsCopyProgressRoutine, &f, &cancelled, COPY_FILE_ALLOW_DECRYPTED_DESTINATION | COPY_FILE_COPY_SYMLINK); // TODO(Cristian): Allow more options?
    return cancelled != TRUE && ret != 0;
}
#else
#error This copy file routine should be implemented for other OS's
#endif