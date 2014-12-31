///
/// Path.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 29 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// A wrapper for a path utility
///

#pragma once

#ifndef Path_h__
#define Path_h__

/// TODO: Add caller-checked preconditions for speedups?

namespace vorb {
    namespace io {
        class Directory;
        class File;

        /// Wrapper path class
        class Path {
        public:
            /// Construct an empty path
            Path();
            /// Construct a path from a string
            /// @param p: Path value
            Path(const cString p);
            /// Construct a path from a string
            /// @param p: Path value
            Path(const nString& p);

            /// @return The path as a string
            const nString& getString() const {
                return m_path;
            }
            /// @return The path as a string
            CALLEE_DELETE const cString getCString() const {
                return m_path.c_str();
            }

            /// @return True if this path has an empty value
            bool isNull() const;
            /// @return True if this path exists
            bool isValid() const;
            /// @return True if this path exists and is a file
            bool isFile() const;
            /// @return True if this path exists and is a directory
            bool isDirectory() const;

            /// Normalizes and places this path value on the root
            /// @return Self
            Path& makeAbsolute();
            /// @return A normalized and rooted path value
            Path asAbsolute() const {
                Path p = *this;
                p.makeAbsolute();
                return p;
            }

            /// Add a string to the end of this path's value
            /// @param s: String addition
            /// @return Self
            Path& append(const nString& s);
            Path& operator+= (const nString& s) {
                return append(s);
            }
            Path operator+ (const nString& s) const {
                Path p = *this;
                p.append(s);
                return p;
            }
            /// Add a separated path value to the end of this path's value
            /// @param dir: Additional path piece
            /// @return Self
            Path& concatenate(const nString& dir);
            Path& operator/= (const nString& dir) {
                return concatenate(dir);
            }
            Path operator/ (const nString& dir) const {
                Path p = *this;
                p.concatenate(dir);
                return p;
            }
            /// Trims the last entry of this path away
            /// @return Self
            Path& trimEnd();
            Path& operator-- () {
                return trimEnd();
            }
            Path operator-- (int) {
                Path p = *this;
                this->trimEnd();
                return p;
            }

            /// Attempt to convert this path to a directory
            /// @param dir: Pointer to return value if a conversion was possible
            /// @return True if this path was converted successfully
            bool asDirectory(OUT Directory* dir) const;
        private:
            nString m_path; ///< Path value as a string
        };
    }
}
namespace vio = vorb::io;
typedef vio::Path vpath; ///< Path shorthand

#endif // Path_h__