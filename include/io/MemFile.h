///
/// MemFile.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 12 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef MemFile_h__
#define MemFile_h__

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

#endif // MemFile_h__