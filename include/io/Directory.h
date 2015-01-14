///
/// Directory.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 29 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// A directory wrapper
///

#pragma once

#ifndef Directory_h__
#define Directory_h__

#include "../Events.hpp"
#include "Path.h"

namespace vorb {
    namespace io {
        typedef std::vector<Path> DirectoryEntries; ///< A list of directory entries
        typedef IDelegate<const Path&> DirectoryEntryCallback; ///< Type for a callback function

        /// Represents a directory that houses paths
        class Directory {
            friend class Path;
        public:
            /// Create a null directory
            Directory() : Directory(Path()) {
                // Empty
            }

            /// @return Underlying path
            const Path& getPath() const {
                return m_path;
            }
            /// @return True if this directory exists
            bool isValid() const {
                return m_path.isValid();
            }

            /// Iterate the directory, placing entries into a list
            /// @param l: List where entries will be placed
            /// @return Number of added entries
            ui32 appendEntries(OUT DirectoryEntries& l) const;
            /// Iterate a directory, invoking a function on each entry
            /// @pre: f may not be null
            /// @param f: Invokable function
            void forEachEntry(DirectoryEntryCallback* f) const;
            template<typename F>
            void forEachEntry(F f) const {
                DirectoryEntryCallback* fDel = createDelegate<const Path&>(f);
                forEachEntry(fDel);
                // TODO:  Cannot delete abstract (llvm/clang)
                // delete fDel;
            }

            /// @return True if this directory contains no elements
            bool isEmpty() const;
        private:
            /// Secret-sauce directory builder
            /// @param p: Path value
            Directory(const Path& p);

            Path m_path; ///< Directory's path value
        };
    }
}
namespace vio = vorb::io;
typedef vio::Directory vdir; ///< Directory shorthand

#endif // Directory_h__