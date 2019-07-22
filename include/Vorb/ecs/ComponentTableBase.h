//
// ComponentTableBase.h
// Vorb Engine
//
// Created by Cristian Zaloj on 9 Nov 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ComponentTableBase.h
 * @brief Base class for storing entity-component mappings.
 */

#pragma once

#ifndef Vorb_ComponentTableBase_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ComponentTableBase_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Entity.h"
#include "ECS.h"
#include "../Event.hpp"
#include "../IDGenerator.h"

namespace vorb {
    namespace ecs {
        class ComponentTableBase {
            friend class ECS;
        public:
            /// Default constructor which sets up events
            ComponentTableBase();

            /// @return This table's ID within an ECS
            const TableID& getID() const {
                return m_id;
            }

            /// @return Iterator to the first pair of (entity ID, component ID)
            ComponentBindingSet::iterator begin() {
                return _components.begin();
            }
            /// @return Iterator to the end of component pairing list
            ComponentBindingSet::iterator end() {
                return _components.end();
            }
            /// @return Const iterator to the first pair of (entity ID, component ID)
            ComponentBindingSet::const_iterator cbegin() const {
                return _components.cbegin();
            }
            /// @return Const iterator to the end of component pairing list
            ComponentBindingSet::const_iterator cend() const {
                return _components.cend();
            }

            /// Obtain the component ID for this entity
            /// @param eID: ID of entity to search
            /// @return Component ID if it exists, else ID_GENERATOR_NULL_ID
            ComponentID getComponentID(EntityID eID) const {
                auto comp = _components.find(eID);
                if (comp == _components.end()) return ID_GENERATOR_NULL_ID;
                return comp->second;
            }

            /// @return Number of active components
            size_t getComponentCount() const {
                return _components.size(); // This should be equal to _genComponent.getActiveCount()
            }

            void unsafeSetSize(size_t n);
            void unsafeSetLink(ECS& ecs, EntityID, ComponentID);

            Event<ComponentID, EntityID> onEntityAdded; ///< Called when an entity is added to this table
            Event<ComponentID, EntityID> onEntityRemoved; ///< Called when an entity is removed from this table
        protected:
            virtual void addComponent(ComponentID cID, EntityID eID) = 0;
            virtual void setComponent(ComponentID cID, EntityID eID) = 0;

            virtual void initComponent(ComponentID cID, EntityID eID) = 0;
            virtual void disposeComponent(ComponentID cID, EntityID eID) = 0;
        private:
            /// Registers a component for this entity
            /// @param eID: Entity ID
            /// @return Registered component ID
            /// @throws std::runtime_error: When an entity already has a registered component
            ComponentID add(EntityID eID);
            /// Removes an entity's component
            /// @param eID: Entity ID
            /// @return True if a component was removed
            bool remove(EntityID eID);

            TableID m_id; ///< ID within a system
            ComponentBindingSet _components; ///< List of (entity ID, component ID) pairings
            vcore::IDGenerator<ComponentID> _genComponent; ///< Unique ID generator
        };
    }
}
namespace vecs = vorb::ecs;

#endif // !Vorb_ComponentTableBase_h__
