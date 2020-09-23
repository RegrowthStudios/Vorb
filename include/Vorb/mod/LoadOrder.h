//
// LoadOrder.h
// Vorb Engine
//
// Created by Matthew Marshall on 3 April 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file LoadOrder.h
 * \brief Handles load order of mods.
 */

#pragma once

#ifndef Vorb_LoadOrderManager_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_LoadOrderManager_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/io/IOManager.h"
#include "Vorb/io/Keg.h"
#include "Vorb/mod/Mod.h"

namespace vorb {
    namespace mod {
        struct LoadOrderProfile {
            // TODO(Matthew): Fill this out with more data points as we go.
            nString name; ///< Name of load order profile.
            nString createdTimestamp; ///< Profile creation timestamp.
            nString lastModifiedTimestamp; ///< Profile last modified timestamp.

            Array<nString> mods; ///< List of mods by name in load order (first in
                                 ///  array is therefore lowest priority: the later
                                 ///  mods will overwrite the earlier ones).
        };
        using LoadOrderProfiles = Array<LoadOrderProfile>;
        struct LoadOrders {
            nString  currentProfile; ///< The name of the currently installed profile.
            LoadOrderProfiles profiles; ///< List of profiles.
        };

        const nString LOAD_ORDER_PROFILES_FILENAME = "load_order_profiles.yaml";

        enum class Action;

        struct ActionForMod;
        using ActionsForMods = std::vector<ActionForMod>;

        class ModEnvironmentBase;

        class LoadOrderManager {
        public:
            LoadOrderManager();

            /*!
             * \brief Initialises the load order manager with the IOManager
             * needed to acquire load orders.
             *
             * \param loadOrderConfigDir: The directory in which the load order config is stored.
             */
            void init(const ModEnvironmentBase* modEnv, const vio::Path& loadOrderConfigDir);
            /*! \brief Disposes the load order manager.
             */
            void dispose();

            bool addLoadOrderProfile(LoadOrderProfile&& profile);
            bool replaceLoadOrderProfile(LoadOrderProfile&& profile);
            bool removeLoadOrderProfile(const nString& name);

            const LoadOrderProfile* getLoadOrderProfile(const nString& name) const;
            const LoadOrderProfile* getCurrentLoadOrderProfile() const;
            const LoadOrderProfiles& getAllLoadOrderProfiles() const;

            void setCurrentLoadOrderProfile(const LoadOrderProfile* profile);
            bool setCurrentLoadOrderProfile(const nString& name);
        protected:
            /*! \brief Acquires all load order profiles currently specified.
             */
            void acquireLoadOrders();

            const ModEnvironmentBase* m_modEnvironment; ///< The mod environment this load order sits in.

            vio::IOManager m_ioManager; ///< The IO manager with which load orders are acquired.

            LoadOrders m_loadOrders; ///< The list of load order profiles.
            const LoadOrderProfile* m_currentLoadOrder; ///< The current load order profile.
        };
    }
}
namespace vmod = vorb::mod;

KEG_TYPE_DECL(LoadOrderProfile);
KEG_TYPE_DECL(LoadOrders);

#endif // !Vorb_LoadOrderManager_h__
