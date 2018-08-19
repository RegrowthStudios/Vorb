//
// Directory.h
// Vorb Engine
//
// Created by Cristian Zaloj on 29 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file Directory.h
 * @brief A directory wrapper.
 */

#pragma once

#ifndef Vorb_Directory_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Directory_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <vector>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "../Events.hpp"
#include "Path.h"

namespace vorb {
    namespace io {
        typedef std::vector<Path> DirectoryEntries; ///< A list of directory entries
        typedef Delegate<Sender, const Path&> DirectoryEntryCallback; ///< Type for a callback function

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
            size_t appendEntries(OUT DirectoryEntries& l) const;
            /// Iterate a directory, invoking a function on each entry
            /// @pre: f may not be null
            /// @param f: Invokable function
            void forEachEntry(DirectoryEntryCallback* f) const;
            template<typename F>
            void forEachEntry(F f) const {
                DirectoryEntryCallback* fDel = makeFunctor(f);
                forEachEntry(fDel);
                delete fDel;
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

#endif // !Vorb_Directory_h__
