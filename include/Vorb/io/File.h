//
// File.h
// Vorb Engine
//
// Created by Cristian Zaloj on 31 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file File.h
 * @brief A file wrapper.
 */

#pragma once

#ifndef Vorb_File_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_File_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Path.h"

namespace vorb {
    namespace io {
        class FileStream;

        /// Different modes for opening a file
        enum class FileOpenFlags {
            NONE = 0x00, ///< No flags
            BINARY = 0x01, ///< Binary file mode (no conversions)

            CREATE = 0x08, ///< File has creation flags

            READ_ONLY_EXISTING = 0x02, ///< Open file for reading
            READ_WRITE_EXISTING = 0x04, ///< Open file with full permissions

            WRITE_ONLY_CREATE = CREATE | 0x00, ///< Create a file for writing only
            READ_WRITE_CREATE = CREATE | 0x02, ///< Create a file with full permissions
            
            WRITE_ONLY_APPEND = CREATE | 0x04, ///< Open/create a file with write permissions at the end
            READ_WRITE_APPEND = CREATE | 0x06 ///< Open/create a file with full permissions at the end
        };
        ENUM_CLASS_OPS_INL(FileOpenFlags, ui8)

//        // TODO: Maybe move sum elsewhere?
//        struct SHA256Sum {
//            const ui32& operator[](const size_t& i) const {
//                return m_data[i];
//            }
//            ui32& operator[](const size_t& i) {
//                return m_data[i];
//            }
//        private:
//            ui32 m_data[8];
//        };

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
//            /// @return A file checksum
//            void computeSum(OUT SHA256Sum* sum) const;

            /// @return The size of the file in bytes
            ui64 length() const;
            
            /// Attempt to resize this file
            /// @param l: New file size
            /// @return True if the file was resized successfully
            bool resize(const ui64& l) const;

            /// Open the file handle
            /// @return A stream to the file
            FileStream open(FileOpenFlags flags) const;
            /// Open the file handle
            /// @return A stream to the file
            FileStream open(const bool& binary = true) const;
            /// Open the file handle for reading only
            /// @return A stream to the file
            FileStream openReadOnly(const bool& binary = true) const;
            /// Create the file handle
            /// @return A stream to the file
            FileStream create(const bool& binary = true) const;
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

#endif // !Vorb_File_h__
