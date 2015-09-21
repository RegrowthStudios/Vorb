//
// FileOps.h
// Vorb Engine
//
// Created by Cristian Zaloj on 1 Jan 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file FileOps.h
 * @brief Operations with the file system.
 */

#pragma once

#ifndef Vorb_FileOps_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FileOps_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Path.h"
#include "../Events.hpp"

namespace vorb {
    namespace io {
        typedef RDelegate<bool, ui64, ui64> CopyFileProgressCallback;

        bool buildDirectoryTree(const Path& path, bool omitEnd = false);
        
        bool copyWithProgress(const Path& src, const Path& dest, CopyFileProgressCallback f);
    }
}
namespace vio = vorb::io;

#endif // !Vorb_FileOps_h__
