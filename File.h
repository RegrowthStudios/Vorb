///
/// File.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 31 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// A file wrapper
///

#pragma once

#ifndef File_h__
#define File_h__

#include <memory>

#include "Path.h"
#include "FileStream.h"

namespace vorb {
    namespace io {
        /// Represents a file
        class File {
            friend class Path;
        public:
            /// Create a null file
            File() : File(Path()) {
                // Empty
            }

            /// @return Underlying path
            const Path& getPath() const {
                return m_path;
            }
            /// @return True if this file exists
            bool isValid() const {
                return m_path.isValid();
            }

            /// @return The size of the file in bytes
            ui64 getSize() const;
            
            /// Attempt to resize this file
            /// @param l: New file size
            /// @return True if the file was resized successfully
            bool resize(ui64 l) const;

            /// Open the file handle
            /// @return A stream to the file
            FileStream open(bool binary = true);
            /// Open the file handle for reading only
            /// @return A stream to the file
            FileStream openReadOnly(bool binary = true);
            /// Create the file handle
            /// @return A stream to the file
            FileStream create(bool binary = true);
        private:
            /// Secret-sauce file builder
            /// @param p: Path value
            File(const Path& p);

            Path m_path; ///< File's path value
        };
    }
}
namespace vio = vorb::io;
typedef vio::File vfile; ///< File shorthand

#endif // File_h__