//
// MultiComponentTracker.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 1 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file MultiComponentTracker.hpp
 * @brief An extended entity tracker.
 */

#pragma once

#ifndef Vorb_MultiComponentTracker_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MultiComponentTracker_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include <memory>
#include <unordered_map>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "MultipleComponentSet.h"

namespace vorb {
    namespace ecs {
        /// Tracks entities in specified component sets as well as their component IDs
        template<size_t N> 
        class MultiComponentTracker : public MultipleComponentSet {
        public:
            /// A list of component IDs
            struct Components {
                friend class MultiComponentTracker;
            public:
                /// Obtain a component ID for a specific table
                /// @param i: Index of tracked component table 
                /// @return Component ID
                const ComponentID& operator[] (size_t i) const {
                    return _data[i];
                }
                /// @return Pointer to component IDs
                explicit operator const ComponentID* () const {
                    return _data;
                }
            private:
                /// Update a component ID for a specific table
                /// @param i: Index of component table
                /// @param cID: New component ID
                void set(size_t i, const ComponentID& cID) {
                    _data[i] = cID;
                }

                ComponentID _data[N]; ///< Component ID array
            };

            /// Default constructor that initializes hooks into itself
            MultiComponentTracker() : MultipleComponentSet() {
                // TODO(Matthew): Maybe makeFunctor?
                _fEntityAdded.reset(makeDelegate([&] (Sender sender, EntityID id) {
                    Components comp;
                    for (size_t i = 0; i < N; i++) {
                        comp.set(i, _tables[i]->getComponentID(id));
                    }
                    _trackedComponents.insert(std::make_pair(id, comp));
                }));
                onEntityAdded += _fEntityAdded.get();
                
                _fEntityRemoved.reset(makeDelegate([&] (Sender sender, EntityID id) {
                    _trackedComponents.erase(id);
                }));
                onEntityRemoved += _fEntityRemoved.get();
            }

            /// Obtain the tracked components for an entity
            /// @param id: Tracked entity
            /// @return A list of tracked component ids
            const Components& getComponents(const EntityID& id) const {
                return _trackedComponents.at(id);
            }
        private:
            std::unordered_map<EntityID, Components> _trackedComponents; ///< Stores tracked component IDs for each entity
            std::shared_ptr<Delegate<void, Sender, EntityID>> _fEntityAdded; ///< onEntityAdded listener
            std::shared_ptr<Delegate<void, Sender, EntityID>> _fEntityRemoved; ///< onEntityRemoved listener
        };
    }
}
namespace vecs = vorb::ecs;

#endif // !Vorb_MultiComponentTracker_hpp__
