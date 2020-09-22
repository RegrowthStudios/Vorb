//
// ModEnvironment.h
// Vorb Engine
//
// Created by Matthew Marshall on 4 April 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file ModEnvironment.h
 * \brief Provides the environment in which mods can
 * be set up and managed.
 */

#pragma once

#ifndef Vorb_ModEnvironment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ModEnvironment_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/io/Path.h"
#include "Vorb/mod/LoadOrder.h"
#include "Vorb/mod/Mod.h"
#include "Vorb/mod/install/Installer.h"

namespace vorb {
    namespace mod {
        // TODO(Matthew): There may be a way to do partial mod installs, that is
        //      updates. It would be difficult with partial YAML updates but at
        //      file-level it naively sounds fine. Some complication in needing to
        //      know one file's dependency on another. E.g. a script on some asset
        //      being a certain form. But then load order is problem anyway for that.
        enum class Action {
            INSTALL,
            UNINSTALL
        };

        struct ActionForMod {
            Action how;
            const ModBase* mod;
        };

        class ModEnvironmentBase {
        public:
            ModEnvironmentBase() {
                // Empty.
            }
            virtual ~ModEnvironmentBase() {
                // Empty.
            }

            /*!
             * \brief Initialises the mod environment, which means setting up
             * load order manager, preparing environment with currently active
             * mods.
             *
             * \param stagedModDir: The directory in which not-yet-installed mods
             * are located.
             * \param installedModDir: The directory in which installed mods are
             * located.
             * \param loadOrderDir: The directory in which the load order
             * metadata can be located.
             */
            void init(const vio::Path& stagedModDir, const vio::Path& installedModDir, const vio::Path& loadOrderDir);
            /*! \brief Cleans up the mod environment.
             */
            void dispose();

            // TODO(Matthew): Determine start up and shut down processes for mods.
            //                    For scripted this will be a bit more obvious, but YAML this may still be TBD.
            /*!
             * \brief Starts up the mod.
             */
            virtual bool startup() = 0;
            /*!
             * \brief Shuts down the mod.
             */
            virtual bool shutdown() = 0;

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f) = 0;

            /*!
             * \brief Uninstalls the current load order.
             *
             * \return True if the load order was successfully uninstalled,
             * false otherwise.
             */
            bool uninstallCurrentLoadOrder();

            /*!
             * \brief Installs the named load order. This entails determining
             * the minimum changes to be made between current load order and the
             * new load order, and then performing the necessary uninstalls and
             * installs from/to the game's working directory.
             *
             * \return True if the load order was successfully installed,
             * false otherwise.
             */
            bool installLoadOrder(const nString& name);

            /*!
             * \brief Set the directory that staged mods are found in.
             *
             * \param stagedModDir: The directory to set it to.
             */
            void setStagedModDir(const vio::Path& stagedModDir);
            /*! \return A reference to the directory that staged mods are found in.
             */
            const vio::Path& getStagedModDir() const { return m_stagedModDir; }

            /*!
             * \brief Set the directory that installed mods are found in.
             *
             * \param installedModDir: The directory to set it to.
             */
            void setInstalledModDir(const vio::Path& installedModDir);
            /*! \return A reference to the directory that installed mods are found in.
             */
            const vio::Path& getInstalledModDir() const { return m_installedModDir; }

            /*! \return A reference to the load order manager.
             */
            LoadOrderManager& getLoadOrderManager() { return m_loadOrderManager;}
            /*! \return A reference to the load order manager.
             */
            const LoadOrderManager& getLoadOrderManager() const { return m_loadOrderManager; }

            /*! \return A reference to the installed mods.
             */
            const ModBases& getInstalledMods() const { return m_installedMods; }
            /*!
             * \brief Get the named mod as installed.
             *
             * \param name: The name of the mod to get.
             *
             * \return Pointer to the named installed mod if found, nullptr if no matching installed mod found.
             */
            const ModBase* getInstalledMod(const nString& name) const;
            /*! \return A reference to the staged mods.
             */
            const ModBases& getStagedMods() const { return m_stagedMods; }
            /*!
             * \brief Get the named mod as staged.
             *
             * \param name: The name of the mod to get.
             *
             * \return Pointer to the named staged mod if found, nullptr if no matching staged mod found.
             */
            const ModBase* getStagedMod(const nString& name) const;
        protected:
            /*!
             * \brief Prepares the currently installed mods. This entails getting
             * their metadata and running any initialisation they require.
             *
             * Note: This does _not_ constitute enabling them!
             *
             * \return True if the installed mods were prepared successfully,
             * false otherwise.
             */
            virtual bool prepareInstalledMods() = 0;

            LoadOrderManager m_loadOrderManager; ///< Manages all load order profiles.
            install::Installer m_installer; ///< The installer to use for handling the mod installation procedure.

            vio::Path m_installedModDir; ///< The directory in which installed mods are located.
            vio::Path m_stagedModDir; ///< The directory in which not-yet-installed mods are located.

            ModBases m_installedMods; ///< List of currently installed mods.
            ModBases m_stagedMods; ///< List of currently staged mods.
        };

        template <typename ScriptEnvironment = void, typename Enable = void>
        class ModEnvironment;

        template <typename ScriptEnvironment>
        class ModEnvironment<ScriptEnvironment,
                                typename std::enable_if<!std::is_void<ScriptEnvironment>::value>::type>
            : public ModEnvironmentBase {
        public:
            ~ModEnvironment() {
                // Empty.
            }
        protected:
            /*!
             * \brief Prepares the currently active mods. This entails getting
             * their metadata and running any initialisation they require.
             *
             * \return True if the active mods were prepared successfully,
             * false otherwise.
             */
            virtual bool prepareInstalledMods() override;
        };

        template <typename ScriptEnvironment>
        class ModEnvironment<ScriptEnvironment,
                                typename std::enable_if<std::is_void<ScriptEnvironment>::value>::type>
            : public ModEnvironmentBase {
        public:
            ~ModEnvironment() {
                // Empty.
            }
        protected:
            /*!
             * \brief Prepares the currently active mods. This entails getting
             * their metadata and running any initialisation they require.
             *
             * \return True if the active mods were prepared successfully,
             * false otherwise.
             */
            virtual bool prepareInstalledMods() override;
        };
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_ModEnvironment_h__
