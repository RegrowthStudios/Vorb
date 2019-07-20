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
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Path.h"

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
                virtual bool prepare(Installer* installer) = 0;

                /*!
                 * \brief Execute strategy during installation.
                 *
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(Installer* installer) = 0;

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(Installer* installer) = 0;
            protected:
                /*!
                 * \brief Registers an entry point to be extracted from entry points file.
                 *
                 * In this case, the entry point names only one file to be accessed.
                 *
                 * \param installer: The installer that the entry point should be registered to.
                 * \param entryPoint: Name of the entry point to register.
                 */
                void registerSingleEntryPoint(Installer* installer, const nString& entryPoint);
                /*!
                 * \brief Registers an entry point to be extracted from entry points file.
                 *
                 * In this case, the entry point names multiple files to be accessed.
                 *
                 * \param installer: The installer that the entry point should be registered to.
                 * \param entryPoint: Name of the entry point to register.
                 */
                void registerMultiEntryPoint(Installer* installer, const nString& entryPoint);
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
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(Installer* installer);

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(Installer* installer);
            protected:
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
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(Installer* installer);

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(Installer* installer);
            protected:
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
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool install(Installer* installer);

                /*!
                 * \brief Execute strategy during uninstallation.
                 *
                 * \param installer: The installer that this strategy is being executed by.
                 *
                 * \return True if successfully executed, false otherwise.
                 */
                virtual bool uninstall(Installer* installer);
            protected:
                vio::Path dest;      ///< The file into which data is merged.
                nString   listEntry; ///< The field name in the entry-points YAML file from which all files to
                                     ///  use for merging are listed.
            };
        }
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_InstallStrategy_h__
