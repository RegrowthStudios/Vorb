#include "stdafx.h"
#include "io/Directory.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

vio::Directory::Directory(const Path& p) :
    m_path(p) {
    // Empty
}

bool vio::Directory::isEmpty() const {
    fs::path p(m_path.getString());
    return fs::is_empty(p);
}

size_t vio::Directory::appendEntries(DirectoryEntries& l) const {
    fs::path p(m_path.getString());

    size_t c = l.size();
    fs::directory_iterator entry(p);
    fs::directory_iterator END;
    while (entry != END) {
        const fs::path& e = entry->path();
        if (fs::exists(e)) l.emplace_back(e.string());
        entry++;
    }
    return l.size() - c;
}

void vio::Directory::forEachEntry(DirectoryEntryCallback* f) const {
    fs::path p(m_path.getString());

    fs::directory_iterator entry(p);
    fs::directory_iterator END;
    while (entry != END) {
        const fs::path& e = entry->path();
        if (fs::exists(e)) f->invoke(this, Path(e.string()));
        entry++;
    }
}
