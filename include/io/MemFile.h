//
// MemFile.h
// Vorb Engine
//
// Created by Cristian Zaloj on 12 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file MemFile.h
 * @brief A memory-mapped file.
 */

#pragma once

#ifndef Vorb_MemFile_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MemFile_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../compat.h"
#include "../types.h"
#endif // !VORB_USING_PCH

#ifdef OS_WINDOWS
#include <Windows.h>
#include <io.h>
#else
#include <sys/mman.h>
#endif

class MemFile {
public:
    MemFile();
    virtual ~MemFile();

    operator FILE*() const;

    const cString getData() const;
private:
    i32 m_fileDescriptor;
    FILE* m_file;

    cString m_buffer;
};

#endif // !Vorb_MemFile_h__
