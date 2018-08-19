//
// MultipleComponentSet.h
// Vorb Engine
//
// Created by Cristian Zaloj on 30 Nov 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file MultipleComponentSet.h
 * @brief Tracks entities with similar components
 */

#pragma once

#ifndef Vorb_MultipleComponentSet_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MultipleComponentSet_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <memory>
#include <vector>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "Entity.h"
#include "../Events.hpp"

namespace vorb {
    namespace ecs {
        class ComponentTableBase;

        /// Listener class that tracks entities that meet component requirements
        class MultipleComponentSet {
        public:
            /// Default constructor which initializes events
            MultipleComponentSet();
            /// Deletes hooks to registered tables
            ~MultipleComponentSet();

            
            /// Add another component type that all entities in this set must have
            /// @param component: Additional component for criteria testing
            void addRequirement(ComponentTableBase* component);

            /// @return Iterator to the first entity ID
            EntityIDSet::iterator begin() {
                return _entities.begin();
            }
            /// @return Iterator to the end of the entity ID list
            EntityIDSet::iterator end() {
                return _entities.end();
            }
            /// @return Const iterator to the first entity ID
            EntityIDSet::const_iterator cbegin() const {
                return _entities.cbegin();
            }
            /// @return Const iterator to the end of the entity ID list
            EntityIDSet::const_iterator cend() const {
                return _entities.cend();
            }

            Event<EntityID> onEntityAdded; ///< Called when an entity is added to this table
            Event<EntityID> onEntityRemoved; ///< Called when an entity is removed from this table
        protected:
            EntityIDSet _entities; ///< List of entity IDs that meet requirements
            std::vector<ComponentTableBase*> _tables; ///< List of required component tables
        private:
            std::shared_ptr<Delegate<Sender, ComponentID, EntityID>> _fEntityAdded;
            std::shared_ptr<Delegate<Sender, ComponentID, EntityID>> _fEntityRemoved;
        };
    }
}
namespace vecs = vorb::ecs;

#endif // !Vorb_MultipleComponentSet_h__
