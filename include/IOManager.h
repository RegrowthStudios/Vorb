///
/// IOManager.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 2 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef IOManager_h__
#define IOManager_h__

#include "IO.h"

namespace vorb {
    namespace io {
        // Manages Simple IO Operations Within A File System
        class IOManager {
        public:
            // Create An IO Manager With Default Program Directories
            IOManager();

            /// Set up IO environment
            void setSearchDirectory(const Path& s);
            static void setCurrentWorkingDirectory(const Path& s);
            static void setExecutableDirectory(const Path& s);

            // Get The Current Environment State
            const Path& getSearchDirectory() const {
                return m_pathSearch;
            }
            static const Path& getCurrentWorkingDirectory() {
                return m_pathCWD;
            }
            static const Path& getExecutableDirectory() {
                return m_pathExec;
            }

            /// @return The root absolute directory of the path
            static Path getDirectory(const Path& path);

            /// Gets all the entries in a directory
            /// @param dirPath: The directory to search
            /// @param entries: The store for all the resulting paths
            void getDirectoryEntries(const Path& dirPath, OUT DirectoryEntries& entries) const;

            // Attempt To Find An Absolute File Path (Must Be Deleted) With This Environment
            // Returns false on failure
            bool resolvePath(const Path& path, OUT Path& resultAbsolutePath) const;

            // Open A File Using STD Flags ("r", "w", "b", etc.) 
            // Returns NULL If It Can't Be Found
            FileStream openFile(const Path& path, const FileOpenFlags& flags) const;

            // Read An Entire File To A String
            // Returns false If File Can't Be Found
            bool readFileToString(const Path& path, OUT nString& data) const;
            CALLER_DELETE cString readFileToString(const Path& path) const;
            bool readFileToData(const Path& path, OUT std::vector<ui8>& data) const;

            /// Writes a string to a file. Creates file if it doesn't exist
            /// @param path: The path to the file
            /// @param data: The data to write to file
            /// @return true on success
            bool writeStringToFile(const Path& path, const nString& data) const;

            /// Makes a directory
            /// @param path: The directory path to make
            bool makeDirectory(const Path& path) const;

            /// Check if a file exists
            /// @param path: The path to the file
            /// @return true if file exists
            bool fileExists(const Path& path) const;

            /// Check if a directory exists
            /// @param path: The path to the directory
            /// @return true if directory exists
            bool directoryExists(const Path& path) const;
        private:
            // Search Order (Top-Down)
            Path m_pathSearch;
            static Path m_pathCWD;
            static Path m_pathExec;
        };
    }
}
namespace vio = vorb::io;

#endif // IOManager_h__