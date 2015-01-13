#include "stdafx.h"
#include "io/MemFile.h"

MemFile::MemFile() {
    m_file = tmpfile();
    m_fileDescriptor = _fileno(m_file);

#ifdef OS_WINDOWS
    HANDLE fm;
    HANDLE h = (HANDLE)_get_osfhandle(m_fileDescriptor);

    fm = CreateFileMapping(h, nullptr, PAGE_READWRITE | SEC_RESERVE, 0, 16384, nullptr);
    if (fm != NULL) {
        m_buffer = (cString)MapViewOfFile(fm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    }
#else
    m_buffer = mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    if (m_buffer == MAP_FAILED) {
        m_buffer = nullptr;
    }
#endif
}
MemFile::~MemFile() {
    fclose(m_file);
    m_file = nullptr;
    m_buffer = nullptr;
}

MemFile::operator FILE*() const {
    return m_file;
}
