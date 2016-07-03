//
// FileStream.h
// Vorb Engine
//
// Created by Cristian Zaloj on 31 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file FileStream.h
 * 
 * \brief An opened file stream for I/O operations.
 */

#pragma once

#ifndef Vorb_FileStream_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FileStream_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "File.h"

namespace vorb {
    namespace io {
        enum FileSeekAnchor {
            BEGINNING = SEEK_SET,
            CURRENT = SEEK_CUR,
            END = SEEK_END
        };
        typedef long FileSeekOffset;

        /// Represents an opened file stream
        class FileStream {
            friend class File;
        public:
            /// A null stream constructor
            FileStream() : FileStream(File()) {
                // Empty
            }

            /// \return True if the file handle is still present
            bool isOpened() {
                return m_fileCached != nullptr;
            }

            /// \return This stream's file reference
            const File& getFile() const {
                return m_parent;
            }

            /// Flushes, closes and invalidates and copies of this stream
            void close() {
                if (isOpened()) {
                    m_fileCached = nullptr;
                    m_handle->close();
                    m_handle.reset();
                }
            }

            /// Write a chunk of data
            /// \param count: Number of elements
            /// \param size: Size of an element (in bytes)
            /// \param data: Pointer to data
            size_t write(size_t count, size_t size, const void* data) const {
                return fwrite(data, size, count, m_fileCached);
            }
            /// Write a formatted string
            /// \param format: String format
            /// \param args: Format arguments
            template<typename... Args>
            void write(const cString format, Args... args) const {
                fprintf(m_fileCached, format, args...);
            }

            /// Read data to a buffer
            /// \param count: Number of elements
            /// \param size: Size of an element (in bytes)
            /// \param data: Pointer to data buffer
            size_t read(size_t count, size_t size, void* data) const {
                return fread(data, size, count, m_fileCached);
            }

            /// Read a formatted string
            /// \param format: String format
            /// \param args: Format arguments
            template<typename... Args>
            void read(const cString format, Args... args) const {
                fscanf(m_fileCached, format, args...);
            }

            /// Move the read/write head to a certain place
            /// \param off: Offset from anchor in bytes
            /// \param anchor: Reference position in file
            void seek(FileSeekOffset off, const FileSeekAnchor& anchor) const {
                fseek(m_fileCached, off, (i32)anchor);
            }
            /// \return Current offset in the file in bytes
            FileSeekOffset offset() const {
                return ftell(m_fileCached);
            }
            /// \return Length of the file to be read in bytes
            FileSeekOffset length() const {
                seek(0, FileSeekAnchor::END);
                FileSeekOffset l = offset();
                seek(0, FileSeekAnchor::BEGINNING);
                return l - offset();
            }

            /// Flush all written data to disk
            void flush() {
                fflush(m_fileCached);
            }
        private:
            /// Create a file stream from a file
            /// \param parent:
            FileStream(const File& parent) :
                m_parent(parent) {
                // Empty
            }

            /// Proper file resource destructor
            class Handle {
                friend class FileStream;
                friend class File;
            public:
                /// Closes the stream
                ~Handle() {
                    close();
                }
                /// Flush and closes the file
                void close() {
                    if (m_file) {
                        fclose(m_file);
                        m_file = nullptr;
                    }
                }
            private:
                FILE* m_file = nullptr; ///< File pointed by the handle
            };

            std::shared_ptr<Handle> m_handle; ///< Ref-counted file handle
            FILE* m_fileCached = nullptr; ///< Cached file pointer
            File m_parent; ///< File reference
        };
    }
}
namespace vio = vorb::io;
typedef vio::FileStream vfstream; /// FileStream shorthand

#endif // !Vorb_FileStream_h__
