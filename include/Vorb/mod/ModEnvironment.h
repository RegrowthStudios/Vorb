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

namespace vorb {
    namespace mod {
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
             * \param loadOrderDir: The directory in which the load order
             * metadata can be located.
             */
            void init(const vio::Path& globalModDir, const vio::Path& loadOrderDir);
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
             * \brief Deactivates the current load order. This entails
             * uninstalling the associated mods from the game's working
             * directory.
             *
             * \return True if the load order was successfully deactivated,
             * false otherwise.
             */
            bool deactivateCurrentLoadOrder();

            /*!
             * \brief Activates the named load order. This entails
             * installing the associated mods to the game's working directory.
             *
             * \return True if the load order was successfully activated,
             * false otherwise.
             */
            bool activateLoadOrder(const nString& name);

            /*!
             * \brief Set the global mod directory of the mod environment.
             *
             * \param globalModDir: The directory to set it to.
             */
            void setGlobalModDir(const vio::Path& globalModDir);
            /*! \return A reference to the global mod directory path.
             */
            const vio::Path& getGlobalModDir() const { return m_globalModDir; }

            /*! \return A reference to the load order manager.
             */
            LoadOrderManager& getLoadOrderManager() { return m_loadOrderManager;}
            /*! \return A reference to the load order manager.
             */
            const LoadOrderManager& getLoadOrderManager() const { return m_loadOrderManager; }

            /*! \return A reference to the active mods.
             */
            virtual const ModBase& getActiveMods() const = 0;
        protected:
            /*!
             * \brief Prepares the currently active mods. This entails getting
             * their metadata and running any initialisation they require.
             *
             * Note: This does _not_ constitute enabling them!
             *
             * \return True if the active mods were prepared successfully,
             * false otherwise.
             */
            virtual bool prepareActiveMods() = 0;

            LoadOrderManager m_loadOrderManager; ///< Manages all load order profiles.

            vio::Path m_globalModDir; ///< The directory in which active mods are located.
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

            /*! \return A reference to the active mods.
             */
            virtual const ModBase& getActiveMods() const override;
        protected:
            /*!
             * \brief Prepares the currently active mods. This entails getting
             * their metadata and running any initialisation they require.
             *
             * \return True if the active mods were prepared successfully,
             * false otherwise.
             */
            virtual bool prepareActiveMods() override;

            Mods<ScriptEnvironment> m_activeMods; ///< List of currently active mods.
        };

        template <typename ScriptEnvironment>
        class ModEnvironment<ScriptEnvironment,
                                typename std::enable_if<std::is_void<ScriptEnvironment>::value>::type>
            : public ModEnvironmentBase {
        public:
            ~ModEnvironment() {
                // Empty.
            }

            /*! \return A reference to the active mods.
             */
            virtual const ModBase& getActiveMods() const override;
        protected:
            /*!
             * \brief Prepares the currently active mods. This entails getting
             * their metadata and running any initialisation they require.
             *
             * \return True if the active mods were prepared successfully,
             * false otherwise.
             */
            virtual bool prepareActiveMods() override;

            Mods<> m_activeMods; ///< List of currently active mods.
        };
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_ModEnvironment_h__
