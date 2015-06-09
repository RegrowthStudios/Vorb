///
/// FileOps.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 1 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Operations with the file system
///

#pragma once

#ifndef FileOps_h__
#define FileOps_h__

#include "Path.h"

namespace vorb {
    namespace io {
        typedef RDelegate<bool, ui64, ui64> CopyFileProgressCallback;

        bool buildDirectoryTree(const Path& path, bool omitEnd = false);
        
        bool copyWithProgress(const Path& src, const Path& dest, CopyFileProgressCallback f);
    }
}
namespace vio = vorb::io;

#endif // FileOps_h__