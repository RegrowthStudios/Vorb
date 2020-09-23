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
             * \param modDir: The directory in which mods are located.
             * \param loadOrderDir: The directory in which the load order
             * metadata can be located.
             */
            void init(const vio::Path& modDir, const vio::Path& loadOrderDir);
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
             * \brief Deactivates the current load order.
             *
             * \return True if the load order was successfully deactivated,
             * false otherwise.
             */
            bool deactivateCurrentLoadOrder();

            /*!
             * \brief Activates the given load order. This entails determining
             * the minimum changes to be made between current load order and the
             * new load order, and then performing the necessary uninstalls and
             * installs from/to the game's working directory.
             *
             * \param newLoadOrder: The load order to make active.
             *
             * \return True if the load order was successfully activated,
             * false otherwise.
             */
            bool setActiveLoadOrder(const LoadOrderProfile* newLoadOrder);
            /*!
             * \brief Activates the named load order. This entails determining
             * the minimum changes to be made between current load order and the
             * new load order, and then performing the necessary uninstalls and
             * installs from/to the game's working directory.
             * 
             * \param name: The name of the load order to make active.
             *
             * \return True if the load order was successfully activated,
             * false otherwise.
             */
            bool setActiveLoadOrder(const nString& name);

            /*!
             * \brief Set the directory that mods are found in.
             *
             * \param modDir: The directory to set it to.
             */
            void setModDir(const vio::Path& modDir);
            /*! \return A reference to the directory that mods are found in.
             */
            const vio::Path& getModDir() const { return m_modDir; }

            /*! \return A reference to the load order manager.
             */
            LoadOrderManager& getLoadOrderManager() { return m_loadOrderManager;}
            /*! \return A reference to the load order manager.
             */
            const LoadOrderManager& getLoadOrderManager() const { return m_loadOrderManager; }

            /*! \return A reference to all mods the environment is aware of.
             */
            const ModBases& getMods() const { return m_mods; }
            /*!
             * \brief Get the named mod.
             *
             * \param name: The name of the mod to get.
             *
             * \return Pointer to the named mod if found, nullptr if no matching mod found.
             */
            const ModBase* getMod(const nString& name) const;
            /*! \return A reference to the active mods.
             */
            const ModBasePtrs& getActiveMods() const { return m_activeMods; }
            /*!
             * \brief Get the named active mod.
             *
             * \param name: The name of the mod to get.
             *
             * \return Pointer to the named active mod if found, nullptr if no matching active mod found.
             */
            const ModBase* getActiveMod(const nString& name) const;

            /*! \brief Discovers all mods available to the environment.
             */
            void discoverMods();
        protected:
            /*! \brief Registers a new mod to the installed mods list.
             */
            virtual void registerMod(ModMetadata metadata, const vio::Path& dir) = 0;

            vio::IOManager  m_ioManager;  ///< The IO manager used for file handling.

            LoadOrderManager m_loadOrderManager; ///< Manages all load order profiles.
            install::Installer m_installer; ///< The installer to use for handling the mod installation procedure.

            vio::Path m_modDir; ///< The directory in which installed mods are located.

            ModBases m_mods; ///< List of all mods the environment is aware of.
            ModBasePtrs m_activeMods; ///< List of all active mods according to the current load order.
        };

        template <typename ScriptEnvironment>
        class ModEnvironment
            : public ModEnvironmentBase {
        public:
            using ScriptEnvBuilder = Delegate<ScriptEnvironment*>;
            using ScriptEnvBuilders = std::vector<ScriptEnvBuilder>;

            ~ModEnvironment() {
                // Empty.
            }

            /*!
             * \brief Registers a script env builder to be used for building a mod's script environment.
             *
             * Note: These builders should be registered by subsystems in order for them to register constants, C callbacks
             * and so forth to the script environment.
             *
             * \param scriptEnvBuilder The script environment builder to be registered.
             */
            template <typename = typename std::enable_if<!std::is_void<ScriptEnvironment>::value>::value>
            void registerScriptEnvBuilder(ScriptEnvBuilder&& scriptEnvBuilder) {
                m_scriptEnvBuilders.emplace_back(std::forward<ScriptEnvBuilder>(scriptEnvBuilder));
            }
            /*! \return The list of script environment builders used for building each mod's script environment.
             */
            const ScriptEnvBuilders& getScriptEnvBuilders() {
                return m_scriptEnvBuilders;
            }
        protected:
            /*! \brief Registers a new mod to the mods list.
             */
            virtual void registerMod(ModMetadata metadata, const vio::Path& dir) override;

            /*! \brief Builds a script environment for use by a mod.
             */
            ScriptEnvironment* buildScriptEnv();

            ScriptEnvBuilders m_scriptEnvBuilders; ///< The list of builders to use in building a script environment for mods.
        };

        template <typename ScriptEnvironment>
        void ModEnvironment<ScriptEnvironment>::registerMod(ModMetadata metadata, const vio::Path& dir) {
            Mod<ScriptEnvironment> newMod;

            if (!std::is_same<ScriptEnvironment, void>::value) {
                ScriptEnvironment* scriptEnv = buildScriptEnv();
                newMod.init(metadata, dir, scriptEnv);
            } else {
                newMod.init(metadata, dir);
            }
        }

        template <typename ScriptEnvironment>
        ScriptEnvironment* ModEnvironment<ScriptEnvironment>::buildScriptEnv() {
            ScriptEnvironment* scriptEnv = new ScriptEnvironment();

            for (auto& builder : m_scriptEnvBuilders) {
                builder(scriptEnv);
            }

            return scriptEnv;
        }
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_ModEnvironment_h__
