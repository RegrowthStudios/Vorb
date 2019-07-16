//
// ModIOManager.h
// Vorb Engine
//
// Created by Matthew Marshall on 16 July 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file ModdIOManager.h
 * \brief An IO manager for mods. This IO manager is specifically designed for usage as the underlying utility
 * to search mod directories and write to those directories. It permits only search and writing relative paths
 * within directories. Further, those directories have associated permissions so that a mod can't, for example,
 * overwrite game files.
 */

#pragma once

#ifndef Vorb_ModIOManager_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ModIOManager_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Vorb/io/Directory.h"
#include "Vorb/io/File.h"
#include "Vorb/io/FileStream.h"
#include "Vorb/io/Path.h"

namespace vorb {
    namespace mod {
        /*!
         * \brief Manages simple IO operations within a file system with restrictions on searchable directories.
         *  
         * When attempting any operations, it looks only in the list of specified searchable directories. Valid paths
         * must all be relative to one of these searchable directories. The prioritisation of these directories is in
         * reverse order to their insertion into the list.
         */
        class ModIOManager {
        public:
            /*!
             * \brief Create an IO manager with default program directories.
             * 
             * It uses the current working directory as its search directory.
             */
            ModIOManager();

            /*!
             * \brief Sets the mod directory of this mod IO manager.
             * 
             * \param path: New mod directory.
             */
            void setModDirectory(const vio::Path& path);
            /*!
             * \brief Sets the vanilla asset directory of this mod IO manager.
             *
             * If called with no path, it will set the vanilla asset directory to a Null path.
             *
             * \param path: New vanilla asset directory.
             */
            void setVanillaAssetDirectory(const vio::Path& path = vio::Path(""));
            /*!
             * \brief Sets the global mod directory of all mod IO managers.
             *
             * \param path: New global mod directory.
             */
            static void setGlobalModDirectory(const vio::Path& path);

            /*!
             * \brief Find the absolute description of a path.
             * 
             * If a path is already absolute, this method return false.
             * 
             * \param path: The path to search.
             * \param resultAbsolutePath: The resulting absolute path will be stored here.
             *
             * \return True if the path exists and was resolved as within one of the searchable directories.
             */
            bool resolvePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath) const;
            /*!
             * \brief Assure the existence of a path if it is nicely formed.
             *
             * If a path is already absolute, this method will return false.
             *
             * Note: Assuring a path will, where necessary and possible, create a file. This will therefore
             * only assure paths within the mod's own directory.
             *
             * \param path: The path to assure.
             * \param resultAbsolutePath: The resulting absolute path will be stored here.
             * \param isFile: If the path does not exist, true to create a file, else create a directory.
             * \param wasExisting: The predication of the path's existence will be stored here if non-null.
             *
             * \return True if the path was resolved properly as within one of the searchable directories.
             */
            bool assurePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting = nullptr) const;

            /*!
             * \brief Opens a file using the standard flags ("r", "w", "b", etc.).
             * 
             * Note: the mode chosen for opening a file also decides which of the searchable directories
             * are used for finding and opening the named file. For example, if a writeable mode is chosen,
             * then only the mod's own directory will be searchable.
             * 
             * \param path: The path to the file to be opened.
             * \param flags: The flags for choosing how the file be opened.
             *
             * \return An empty file stream if the file couldn't be opened, otherwise a valid file stream.
             */
            vio::FileStream openFile(const vio::Path& path, const vio::FileOpenFlags& flags) const;

            /*!
             * \brief Reads a file to the provided string.
             *
             * \param path: The path to the file to be read.
             * \param data: The string in which to place the read file contents.
             *
             * \return True if the file is found and read, false otherwise.
             */
            bool readFileToString(const vio::Path& path, OUT nString& data) const;
            /*!
             * \brief Reads a file to a C-string.
             *
             * \param path: The path to the file to be read.
             *
             * \return A \0 terminated C-string of the file's contents if read, nullptr otherwise.
             */
            CALLER_DELETE cString readFileToString(const vio::Path& path) const;
            /*!
             * \brief Reads a file to the provided string.
             *
             * \param path: The path to the file to be read.
             * \param data: The vector of chars in which to place the read file contents.
             *
             * \return True if the file is found and read, false otherwise.
             */
            bool readFileToData(const vio::Path& path, OUT std::vector<ui8>& data) const;

            /*!
             * \brief Writes a string to the given file. If the file does not yet exist, it is created.
             * 
             * Note: This is a naive function, and it always appends where possible.
             * 
             * Note: This function can only write to a file within the mod's own directory.
             *
             * \param path: The path to the file to be written.
             * \param data: The data to write to the file.
             */
            bool writeStringToFile(const vio::Path& path, const nString& data) const;

            /*!
             * \brief Creates a directory.
             *
             * Note: This function can only create a directory within the mod's own directory.
             *
             * \param path: The path of the directory to make.
             *
             * \return True if the directory was made, false otherwise.
             */
            bool makeDirectory(const vio::Path& path) const;

            /*!
             * \brief Checks if the named file exists.
             *
             * \param path: The path of the file to check for existence.
             *
             * \return True if the file exists, false otherwise.
             */
            bool fileExists(const vio::Path& path) const;

            /*!
             * \brief Checks if the named directory exists.
             *
             * \param path: The path of the directory to check for existence.
             *
             * \return True if the directory exists, false otherwise.
             */
            bool directoryExists(const vio::Path& path) const;
        private:
            static vio::Path m_globalModDir; ///< The global mod directory (Mods/) - this is read-only.

            vio::Path m_vanillaAssetDir; ///< The vanilla asset directory (e.g. Models/, Textures/ etc.) that may be needed for texture cache etc. - this is read-only and can be Null.
            vio::Path m_modDir; ///< The mod's own directory (Mods/MOD_NAME/) - this is read-writable.
        };
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_ModIOManager_h__
