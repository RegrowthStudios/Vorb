//
// IOManager.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file IOManager.h
 * \brief A simpler utility class used for easy call to manipulate IO.
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
        /*! \brief The directory types through which an IOManager searches.
         */
        enum class IOManagerDirectory {
            /*! \brief The first directory on the manager's search pattern.
             * 
             * This is local property stored in IOManager instances.
             */
            SEARCH = 0,
            /*! \brief The second directory on the manager's search pattern.
             * 
             * This is usually the path used by the OS to resolve paths.
             */
            CURRENT_WORKING = 1,
            /*! \brief The third directory on the manager's search pattern.
             * 
             * This is usually the directory in which the program resides.
             */
            EXECUTABLE = 2
        };

        /*!
         * \brief Provides a base for IO management.
         */
        class IOManagerBase {
        public:
            /*! \brief Initialises an IOManagerBase instance.
             */
            IOManagerBase() {
                // Empty.
            }
            virtual ~IOManagerBase() {
                // Empty.
            }

            /*! \brief Find the absolute description of a path.
             * 
             * If a path is already absolute, this method will not attempt to 
             * go through the list of directories and test path combinations.
             * 
             * \param path: The path to search.
             * \param resultAbsolutePath: The resulting absolute path will be stored here.
             * \return True if the path exists and was resolved properly in this environment.
             */
            virtual bool resolvePath(const Path& path, OUT Path& resultAbsolutePath) const = 0;
            /*! \brief Assure the existence of a path if it is nicely formed.
             *
             * If a path is already absolute, this method will not attempt to
             * go through the list of directories and test path combinations.
             *
             * \param path: The path to assure.
             * \param resultAbsolutePath: The resulting absolute path will be stored here.
             * \param creationDirectory: The directory type to create the path if it does not exist.
             * \param isFile: If the path does not exist, true to create a file, else create a directory.
             * \param wasExisting: The predication of the path's existence will be stored here if non-null.
             * \return True if the path was resolved properly in this environment.
             */
            virtual bool assurePath(const Path& path, OUT Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting = nullptr) const = 0;

            /*!
             * \brief Open a file using standard flags ("r", "w", "b", etc.).
             * 
             * \param path: The path to the file to open.
             * \param flags: The flags with which to open the file.
             * 
             * \return File stream, empty if file couldn't be opened. 
             */
            FileStream openFile(const Path& path, FileOpenFlags flags = FileOpenFlags::READ_ONLY_EXISTING) const;

            /*!
             * \brief Reads an entire file to a string.
             *
             * \param path: The path to the file to read.
             * \param data: The string buffer to populate.
             * 
             * \return True if the file was read, false otherwise.
             */
            bool readFileToString(const Path& path, OUT nString& data) const;
            /*!
             * \brief Reads an entire file to a  C-string.
             *
             * \param path: The path to the file to read.
             * 
             * \return Pointer to C-string buffer if file read successfully, nullptr otherwise.
             */
            CALLER_DELETE cString readFileToString(const Path& path) const;

            /*!
             * \brief Read an entire file to a binary buffer.
             *
             * \param path: The path to the file to read.
             * \param data: The binary buffer to populate.
             *
             * \return True if the file was read, false otherwise.
             */
            bool readFileToData(const Path& path, OUT std::vector<ui8>& data) const;

            /*!
             * \brief Write a string to a file.
             *
             * \param path: The path to the file to write to.
             * \param data: The data to write to the file.
             * \param flags: The flags with which to write the string.
             *
             * \return True if the data was written, false otherwise.
             */
            bool writeStringToFile(const Path& path, const nString& data, FileOpenFlags flags = FileOpenFlags::WRITE_ONLY_APPEND) const;

            /*!
             * \brief Create directories (recursively) to satisfy the path.
             * This is essentially a simplified call to assurePath.
             *
             * \param path: The path to the deepest directory to create.
             *
             * \return True if the directories were created, false otherwise.
             */
            bool makeDirectory(const Path& path) const;

            /*!
             * \brief Renames/moves a file or directory to a new location.
             *
             * \param src: The path to the source file or directory.
             * \param dest: The path to the destination for this file or directory.
             * \param force: If true, then the rename is forced - any existing file
             * or directory is first removed.
             *
             * NOTE: Forced renames are NOT atomic.
             *
             * \return bool True if successfully renamed/moved, false otherwise.
             */
            bool rename(const Path& src, const Path& dest, bool force = false);

            /*!
             * \brief Determining if the file exists.
             *
             * \param path: The path of the file to determine the existence of.
             *
             * \return True if the file exists, false otherwise.
             */
            bool fileExists(const Path& path) const;

            /*!
             * \brief Determining if the directory exists.
             *
             * \param path: The path of the directory to determine the existence of.
             *
             * \return True if the directory exists, false otherwise.
             */
            bool directoryExists(const Path& path) const;
        };

        /*! \brief Manages simple IO operations within a file system. 
         *  
         * When attempting any operations, it looks not only in the working directory specified by the program,
         * but it also attempts to search in 2 other places. The order of importance and resolution for these directories
         * is: Search, Current Working Directory, and Executing.
         */
        class IOManager: public IOManagerBase {
        public:
            /*! \brief Create an IO manager with default program directories.
             * 
             * It uses the current working directory as its search directory.
             */
            IOManager();
            /*! \brief Create an IO manager that searches within a specified directory.
             * 
             * \param path: The path used as the search directory.
             * \param searchOnly: Whether to only use search directory in path
             * resolution and assurance
             */
            IOManager(const Path& path, bool searchOnly = false);
            virtual ~IOManager() {
                // Empty.
            }

            /*! \brief Change the searching directory for this manager.
             * 
             * \param s: New directory used for first search.
             */
            void setSearchDirectory(const Path& s);
            /*! \brief Change the current working directory referenced by all managers.
             * 
             * \param s: New current working directory.
             */
            static void setCurrentWorkingDirectory(const Path& s);
            /*! \brief Change the executable directory referenced by all managers.
             * 
             * \param s: New executable directory.
             */
            static void setExecutableDirectory(const Path& s);

            /*! \brief Set whether to use search directory only for path resolution
             * and assurance.
             *
             * \param searchOnly: True if so, false if not.
             */
            void setSearchOnly(bool searchOnly);

            /*! \return The search directory used by this manager.
             */
            const Path& getSearchDirectory() const {
                return m_pathSearch;
            }
            /*! \return The current working directory known to all managers.
             */
            static const Path& getCurrentWorkingDirectory() {
                return m_pathCWD;
            }
            /*! \return The executable directory known to all managers.
             */
            static const Path& getExecutableDirectory() {
                return m_pathExec;
            }

            /*! \return Whether we are using search directory only in path resolution
             * and assurance.
             */
            bool isSearchOnly() const {
                return m_searchOnly;
            }

            /*! \brief Obtain all the path entries in a directory.
             * 
             * \param dirPath: The directory to search.
             * \param entries: The store for which all the resulting paths are appended.
             */
            void getDirectoryEntries(const Path& dirPath, OUT DirectoryEntries& entries) const;

            /*! \brief Find the absolute description of a path.
             * 
             * If a path is already absolute, this method will not attempt to 
             * go through the list of directories and test path combinations.
             * 
             * \param path: The path to search.
             * \param resultAbsolutePath: The resulting absolute path will be stored here.
             * \return True if the path exists and was resolved properly in this environment.
             */
            virtual bool resolvePath(const Path& path, OUT Path& resultAbsolutePath) const override;
            /*! \brief Assure the existence of a path if it is nicely formed.
             *
             * If a path is already absolute, this method will not attempt to
             * go through the list of directories and test path combinations.
             *
             * \param path: The path to assure.
             * \param resultAbsolutePath: The resulting absolute path will be stored here.
             * \param isFile: If the path does not exist, true to create a file, else create a directory.
             * \param wasExisting: The predication of the path's existence will be stored here if non-null.
             * \return True if the path was resolved properly in this environment.
             */
            virtual bool assurePath(const Path& path, OUT Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting = nullptr) const override;
            /*! \brief Assure the existence of a path if it is nicely formed.
             *
             * If a path is already absolute, this method will not attempt to
             * go through the list of directories and test path combinations.
             *
             * \param path: The path to assure.
             * \param resultAbsolutePath: The resulting absolute path will be stored here.
             * \param creationDirectory: The directory type to create the path if it does not exist.
             * 
             * \param isFile: If the path does not exist, true to create a file, else create a directory.
             * \param wasExisting: The predication of the path's existence will be stored here if non-null.
             * \return True if the path was resolved properly in this environment.
             */
            bool assurePath(const Path& path, OUT Path& resultAbsolutePath, IOManagerDirectory creationDirectory, bool isFile, OPT bool* wasExisting = nullptr) const;
        private:
            static Path m_pathCWD; ///< The global current working directory.
            static Path m_pathExec; ///< The global executable directory.

            Path m_pathSearch; ///< The first path used in the searching process.

            bool m_searchOnly; ///< If true, then only search directory is used for path
                               ///  resolution and assurance.
        };
    }
}
namespace vio = vorb::io;

#endif // !Vorb_IOManager_h__
