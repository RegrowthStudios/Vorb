//
// DataAssetIOManager.h
// Vorb Engine
//
// Created by Matthew Marshall on 23 Sept 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file DataAssetIOManager.h
 * \brief An IO manager for reading data assets, this manager is load-order-aware and
 * handles locating the most relevant version of a data asset in this context.
 */

#pragma once

#ifndef Vorb_DataAssetIOManager_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_DataAssetIOManager_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/IOManager.h"
#include "Vorb/io/Path.h"

DECL_VMOD(class ModEnvironmentBase; class ModBase)

namespace vorb {
    namespace mod {
        /*!
         * \brief An IO manager for reading data assets, this manager is load-order-aware
         * and handles locating the most relevant version of a data asset in this context.
         * 
         * When a merge strategy is used, caching is performed within a load-order-specific
         * directory, which is the highest-priority read directory.
         *
         * This IO Manager MUST be used on a single thread for security.
         */
        class DataAssetIOManager: public vio::IOManagerBase {
        public:
            /*!
             * \brief Create an IO manager with default program directories.
             *
             * No load order is yet available so only vanilla assets are acquired.
             */
            DataAssetIOManager();

            /*!
             * \brief Sets the global mod directory.
             *
             * \param globalModDir: New global mod directory.
             */
            static void setGlobalModDirectory(const vio::Path& globalModDir);
            /*! \brief Returns the global mod directory. */
            static const vio::Path& getGlobalModDirectory() { return globalModDir; }

            /*!
             * \brief Set the mod environment in which we operate.
             *
             * \param modEnv: The mod environment in which we operate.
             */
            void setModEnvironment(const ModEnvironmentBase* modEnv);

            /*!
             * \brief Set the vanilla data directory for all data asset IO
             * managers.
             *
             * This directory path should be absolute.
             *
             * \param vanillaDataDir: The vanilla data directory.
             */
            static void setVanillaDataDir(const vio::Path& vanillaDataDir);

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
            virtual bool resolvePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath) const override;
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
            virtual bool assurePath(const vio::Path& path, OUT vio::Path& resultAbsolutePath, bool isFile, OPT bool* wasExisting = nullptr) const override;

            /*!
             * \brief Reads file at given path to string within the given mod's
             * directory.
             *
             *
             * \param path: The path to the file to read.
             * \param data: The string buffer to populate.
             * \param mod: The mod whose file to load.
             *
             * \return True if the file was read, false otherwise.
             */
            bool readModFileToString(const vio::Path& path, OUT nString& data, const ModBase* mod);
            /*!
             * \brief Reads file at given path to C-string within the given mod's
             * directory.
             *
             * \param path: The path to the file to read.
             * \param mod: The mod whose file to load.
             *
             * \return Pointer to C-string buffer if file read successfully, nullptr otherwise.
             */
            CALLER_DELETE cString readModFileToString(const vio::Path& path, const ModBase* mod);

            /*!
             * \brief Reads file at given path to binary buffer within the given
             * mod's directory.
             *
             * \param path: The path to the asset to read.
             * \param data: The binary buffer to populate.
             * \param mod: The mod whose file to load.
             *
             * \return True if the asset was read, false otherwise.
             */
            bool readModFileToData(const vio::Path& path, OUT std::vector<ui8>& data, const ModBase* mod);

            /*!
             * \brief Reads file in vanilla data directory to string.
             *
             *
             * \param path: The path to the file to read.
             * \param data: The string buffer to populate.
             * \param mod: The mod whose file to load.
             *
             * \return True if the file was read, false otherwise.
             */
            bool readVanillaFileToString(const vio::Path& path, OUT nString& data);
            /*!
             * \brief Reads file in vanilla data directory to C-string.
             *
             * \param path: The path to the file to read.
             * \param mod: The mod whose file to load.
             *
             * \return Pointer to C-string buffer if file read successfully, nullptr otherwise.
             */
            CALLER_DELETE cString readVanillaFileToString(const vio::Path& path);

            /*!
             * \brief Reads file in vanilla data directory to binary buffer.
             *
             * \param path: The path to the file to read.
             * \param data: The binary buffer to populate.
             * \param mod: The mod whose file to load.
             *
             * \return True if the file was read, false otherwise.
             */
            bool readVanillaFileToData(const vio::Path& path, OUT std::vector<ui8>& data);

            /*!
             * \brief Reads file as in every mod and vanilla data directories
             * into a vector of strings.
             *
             *
             * \param path: The path to the file to read.
             * \param data: The vector of string buffers to populate.
             * \param mod: The mod whose file to load.
             *
             * \return True if the file was read, false otherwise.
             */
            bool readEachFileToString(const vio::Path& path, OUT std::vector<nString>& data);
            /*!
             * \brief Reads file as in every mod and vanilla data directories
             * into a vector of C-strings.
             *
             * \param path: The path to the file to read.
             * \param mod: The mod whose file to load.
             *
             * \return Vector of C-string buffers of files read, empty if none
             * found.
             */
            std::vector<CALLER_DELETE cString> readEachFileToString(const vio::Path& path);

            /*!
             * \brief Reads file as in every mod and vanilla data directories
             * into a vector of binary buffers.
             *
             * \param path: The path to the file to read.
             * \param data: The binary buffer to populate.
             * \param mod: The mod whose file to load.
             *
             * \return True if the file was read, false otherwise.
             */
            bool readEachFileToData(const vio::Path& path, OUT std::vector<std::vector<ui8>>& data);
        private:
            static vio::Path vanillaDataDir; ///< The vanilla data directory (e.g. Data/) - this is read-only.
            static vio::Path globalModDir; ///< The global mod directory (e.g. Mods/) - this is read-only.

            void setSafeMode(bool safeMode = true);

            const ModEnvironmentBase* m_modEnv; ///< The mod evnironment in which this IO manager operates.

            bool m_safeMode;
        };
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_DataAssetIOManager_h__
