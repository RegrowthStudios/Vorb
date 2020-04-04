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
            LoadOrderProfile  currentProfile; ///< The currently installed profile.
            LoadOrderProfiles profiles; ///< List of profiles currently inactive.
        };

        const nString LOAD_ORDER_PROFILES_FILENAME = "load_order_profiles.yaml";

        class LoadOrderManager {
        public:
            LoadOrderManager();

            /*!
             * \brief Initialises the load order manager with the IOManager
             * needed to acquire load orders.
             *
             * \param loadOrderConfigDir: The directory in which the load order config is stored.
             */
            void init(const vio::Path& loadOrderConfigDir);
            /*! \brief Disposes the load order manager.
             */
            void dispose();

            bool addLoadOrderProfile(LoadOrderProfile&& profile);
            bool replaceLoadOrderProfile(LoadOrderProfile&& profile);
            bool removeLoadOrderProfile(const nString& name);

            const LoadOrderProfile& getLoadOrderProfile(const nString& name) const;
            const LoadOrderProfile& getCurrentLoadOrderProfile() const;
            const LoadOrderProfiles& getAllLoadOrderProfiles() const;
        protected:
            /*! \brief Acquires all load order profiles currently specified.
             */
            void acquireLoadOrders();

            vio::IOManager m_ioManager; ///< The IO manager with which load orders are acquired.

            LoadOrders m_loadOrders; ///< The list of load order profiles.
            /*!
             * The currently active load order profile. If none is active,
             * then this will be set to nullptr.
             */
            LoadOrderProfile m_currentLoadOrder;
        };
    }
}
namespace vmod = vorb::mod;

KEG_TYPE_DECL(LoadOrderProfile);
KEG_TYPE_DECL(LoadOrders);

#endif // !Vorb_LoadOrderManager_h__
