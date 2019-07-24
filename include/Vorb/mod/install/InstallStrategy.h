//
// InstallStrategy.h
// Vorb Engine
//
// Created by Matthew Marshall on 16 July 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file InstallStrategy.h
 * \brief Holds all things needed for a given mod. This includes the mod's metadata, but also
 * basic utilities such as an IO manager, texture cache etc.
 */

#pragma once

#ifndef Vorb_InstallStrategy_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_InstallStrategy_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Path.h"

#include <vector>

DECL_VIO(class IOManager)

namespace vorb {
    namespace mod {
        namespace install {
            class Installer;

            struct InstallStrategy {
                /*!
                 * \brief Prepare for executing strategy during installation. This is called on registration.
                 *
                 * \param installer: The installer that this strategy is being prepared for.
                 *
                 * \return True if successfully prepared, false otherwise.
                 */
                virtual bool prepare(Installer* installer);

                /*!
                 * \brief Execute strategy during installation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(const nString& modName) = 0;

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(const nString& modName) = 0;
            protected:
                /*!
                 * \brief Registers an entry point to be extracted from entry points file.
                 *
                 * In this case, the entry point names only one file to be accessed.
                 *
                 * \param entryPoint: Name of the entry point to register.
                 */
                void registerSingleEntryPoint(const nString& entryPoint);
                /*!
                 * \brief Registers an entry point to be extracted from entry points file.
                 *
                 * In this case, the entry point names multiple files to be accessed.
                 *
                 * \param entryPoint: Name of the entry point to register.
                 */
                void registerMultiEntryPoint(const nString& entryPoint);

                bool ownerIsVanilla(keg::Node node);

                CALLEE_DELETE keg::Node getVanillaOwner();

                vio::IOManager* getIOManager();

                vio::Path getInstallDir();
                vio::Path getUpdateDir();
                vio::Path getGlobalModDir();
                vio::Path getBackupDir();
                vio::Path getManifestDir();

                Installer::EntryData* getEntryData();

                /*!
                 * \brief Loads the manifest data.
                 *
                 * \param pathname: The relative path to the resource for which manifest data should
                 * be obtained.
                 *
                 * \return Root node of manifest data. 
                 */
                keg::Node loadCurrentManifestData(const vio::Path& pathname);

                /*!
                 * \brief Loads the manifest data.
                 *
                 * \param modName: The mod to obtain the manifest data for.
                 * \param pathname: The relative path to the resource for which manifest data should
                 * be obtained.
                 *
                 * \return Root node of manifest data. 
                 */
                keg::Node loadManifestDataOfMod(const nString& modName, const vio::Path& pathname);

                Installer* m_installer;
            };

            /*!
             * \brief Provides a strategy to put a copy of the file with the given filepath in the mod dir
             * in the root game dir, replacing the any existing copy of the file.
             */
            struct ReplaceStrategy : InstallStrategy {
                /*!
                 * \brief Prepare for executing strategy during installation. This is called on registration.
                 *
                 * \param installer: The installer that this strategy is being prepared for.
                 *
                 * \return True if successfully prepared, false otherwise.
                 */
                virtual bool prepare(Installer* installer);

                /*!
                 * \brief Execute strategy during installation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(const nString& modName);

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(const nString& modName);

                vio::Path filepath; ///< The path to the file, should be relative to the root game directory.
            };

            /*!
             * \brief Provides a strategy to put a copy of the directory with the given filepath in the mod dir
             * in the root game dir, replacing the any existing copy of the directory.
             *
             * If a set of extensions are provided, then only those files in the directory with the specified
             * extension are copied into the new directory.
             */
            struct ReplaceInDirStrategy : InstallStrategy {
                /*!
                 * \brief Prepare for executing strategy during installation. This is called on registration.
                 *
                 * \param installer: The installer that this strategy is being prepared for.
                 *
                 * \return True if successfully prepared, false otherwise.
                 */
                virtual bool prepare(Installer* installer);

                /*!
                 * \brief Execute strategy during installation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(const nString& modName);

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(const nString& modName);

                vio::Path            dirpath;   ///< The path to the directory, should be relative to the root
                                                ///  game directory.
                std::vector<nString> extension; ///< A collection of extensions to match files to.
            };

            /*!
             * \brief Provides a strategy to merge data stored in specified YAML files into a destination file.
             */
            struct MergeYAMLStrategy : InstallStrategy {
                /*!
                 * \brief Prepare for executing strategy during installation. This is called on registration.
                 *
                 * \param installer: The installer that this strategy is being prepared for.
                 *
                 * \return True if successfully prepared, false otherwise.
                 */
                virtual bool prepare(Installer* installer);

                /*!
                 * \brief Execute strategy during installation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(const nString& modName);

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param modName: The name of the mod to be installed.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(const nString& modName);

                vio::Path dest;      ///< The file into which data is merged.
                vio::Path listEntry; ///< The field name in the entry-points YAML file from which all files to
                                   ///  use for merging are listed.
            };
        }
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_InstallStrategy_h__
