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

namespace vorb {
    namespace io {
        bool buildDirectoryTree(const Path& path, bool omitEnd = false);
    }
}
namespace vio = vorb::io;

#endif // !Vorb_FileOps_h__
