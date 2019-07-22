//
// IOManager.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file IOManager.h
 * @brief A simpler utility class used for easy call to manipulate IO.
 */

#pragma once

#ifndef Vorb_IOManager_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IOManager_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Directory.h"
#include "File.h"
#include "FileStream.h"
#include "Path.h"

namespace vorb {
    namespace io {
        /*! @brief The directory types through which an IOManager searches.
         */
        enum class IOManagerDirectory {
            /*! @brief The first directory on the manager's search pattern.
             * 
             * This is local property stored in IOManager instances.
             */
            SEARCH = 0,
            /*! @brief The second directory on the manager's search pattern.
             * 
             * This is usually the path used by the OS to resolve paths.
             */
            CURRENT_WORKING = 1,
            /*! @brief The third directory on the manager's search pattern.
             * 
             * This is usually the directory in which the program resides.
             */
            EXECUTABLE = 2
        };

        /*! @brief Manages simple IO operations within a file system. 
         *  
         * When attempting any operations, it looks not only in the working directory specified by the program,
         * but it also attempts to search in 2 other places. The order of importance and resolution for these directories
         * is: Search, Current Working Directory, and Executing.
         */
        class IOManager {
        public:
            /*! @brief Create an IO manager with default program directories.
             * 
             * It uses the current working directory as its search directory.
             */
            IOManager();
            /*! @brief Create an IO manager that searches within a specified directory.
             * 
             * @param path: The path used as the search directory.
             */
            IOManager(const Path& path);

            /*! @brief Change the searching directory for this manager.
             * 
             * @param s: New directory used for first search.
             */
            void setSearchDirectory(const Path& s);
            /*! @brief Change the current working directory referenced by all managers.
             * 
             * @param s: New current working directory.
             */
            static void setCurrentWorkingDirectory(const Path& s);
            /*! @brief Change the executable directory referenced by all managers.
             * 
             * @param s: New executable directory.
             */
            static void setExecutableDirectory(const Path& s);

            /*! @return The search directory used by this manager.
             */
            const Path& getSearchDirectory() const {
                return m_pathSearch;
            }
            /*! @return The current working directory known to all managers.
             */
            static const Path& getCurrentWorkingDirectory() {
                return m_pathCWD;
            }
            /*! @return The executable directory known to all managers.
             */
            static const Path& getExecutableDirectory() {
                return m_pathExec;
            }

            /*! @brief Obtain all the path entries in a directory.
             * 
             * @param dirPath: The directory to search.
             * @param entries: The store for which all the resulting paths are appended.
             */
            void getDirectoryEntries(const Path& dirPath, OUT DirectoryEntries& entries) const;

            /*! @brief Find the absolute description of a path.
             * 
             * If a path is already absolute, this method will not attempt to 
             * go through the list of directories and test path combinations.
             * 
             * @param path: The path to search.
             * @param resultAbsolutePath: The resulting absolute path will be stored here.
             * @return True if the path exists and was resolved properly in this environment.
             */
            bool resolvePath(const Path& path, OUT Path& resultAbsolutePath) const;
            /*! @brief Assure the existence of a path if it is nicely formed.
             *
             * If a path is already absolute, this method will not attempt to
             * go through the list of directories and test path combinations.
             *
             * @param path: The path to assure.
             * @param resultAbsolutePath: The resulting absolute path will be stored here.
             * @param creationDirectory: The directory type to create the path if it does not exist.
             * @param isFile: If the path does not exist, true to create a file, else create a directory.
             * @param wasExisting: The predication of the path's existence will be stored here if non-null.
             * @return True if the path was resolved properly in this environment.
             */
            bool assurePath(const Path& path, OUT Path& resultAbsolutePath, IOManagerDirectory creationDirectory, bool isFile, OPT bool* wasExisting = nullptr) const;

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

            /*!
             * \brief Renames/moves a file or directory to a new location.
             *
             * \param src: The path to the source file or directory.
             * \param dest: The path to the destination for this file or directory.
             *
             * \return bool True if successfully renamed/moved, false otherwise.
             */
            bool rename(const Path& src, const Path& dest);

            /// Check if a file exists
            /// @param path: The path to the file
            /// @return true if file exists
            bool fileExists(const Path& path) const;

            /// Check if a directory exists
            /// @param path: The path to the directory
            /// @return true if directory exists
            bool directoryExists(const Path& path) const;
        private:
            static Path m_pathCWD; ///< The global current working directory.
            static Path m_pathExec; ///< The global executable directory.

            Path m_pathSearch; ///< The first path used in the searching process.
        };
    }
}
namespace vio = vorb::io;

#endif // !Vorb_IOManager_h__
