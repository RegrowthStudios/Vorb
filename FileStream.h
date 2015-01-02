///
/// FileStream.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 31 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// An opened file stream for I/O operations 
///

#pragma once

#ifndef FileStream_h__
#define FileStream_h__

#include <memory>

namespace vorb {
    namespace io {
        /// Represents an opened file stream
        class FileStream {
            friend class File;
        public:
            /// @return True if the file handle is still present
            bool isOpened() {
                return m_fileCached != nullptr;
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
            /// @param count: Number of elements
            /// @param size: Size of an element (in bytes)
            /// @param data: Pointer to data
            void write(size_t count, size_t size, void* data) const {
                fwrite(data, size, count, m_fileCached);
            }
            /// Write a formatted string
            /// @param format: String format
            /// @param args: Format arguments
            template<typename... Args>
            void write(const cString format, Args... args) const {
                fprintf(m_fileCached, format, args...);
            }

            /// Read data to a buffer
            /// @param count: Number of elements
            /// @param size: Size of an element (in bytes)
            /// @param data: Pointer to data buffer
            void read(size_t count, size_t size, void* data) {
                fread(data, size, count, m_fileCached);
            }

            /// Flush all written data to disk
            void flush() {
                fflush(m_fileCached);
            }
        private:
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
        };
    }
}
namespace vio = vorb::io;
typedef vio::FileStream vfstream; /// FileStream shorthand

#endif // FileStream_h__