//
// ModIOManager.h
// Vorb Engine
//
// Created by Matthew Marshall on 16 July 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file ModIOManager.h
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
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Directory.h"
#include "Vorb/io/File.h"
#include "Vorb/io/FileStream.h"
#include "Vorb/io/IOManager.h"
#include "Vorb/io/Path.h"

DECL_VMOD(class DataAssetIOManager)

namespace vorb {
    namespace mod {
        /*!
         * \brief Manages simple mod-specific IO operations within permitted directories.
         *
         * When attempting any write operations, the only permitted root directory is the mod's own
         * specific directory (Mods/MOD_DIR/). When attempting read operations this IO manager defers
         * to the load-order-aware data asset IO manager.
         */
        class ModIOManager: public vio::IOManagerBase {
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
             * \brief Sets the load-order-aware data asset IO manager for read operations.
             *
             * \param dataAssetIOManager: The data asset IO manager.
             */
            void setDataAssetIOManager(const DataAssetIOManager* dataAssetIOManager);

            /*! \brief Returns the mod directory searched by this IO manager. */
            vio::Path getModDirectory() { return m_modDir; }

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
        private:
            const DataAssetIOManager* m_dataAssetIOManager; ///< The load-order-aware data asset IO manager - this is read-only.

            vio::Path m_modDir; ///< The mod's own directory - this is read-writable.
        };
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_ModIOManager_h__
