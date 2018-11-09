//
// ECS.h
// Vorb Engine
//
// Created by Cristian Zaloj on 10 Nov 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ECS.h
 * @brief The main ECS class.
 */

#pragma once

#ifndef Vorb_ECS_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ECS_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <unordered_map>
#include <vector>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "Entity.h"
#include "BitTable.hpp"
#include "../Event.hpp"
#include "../IDGenerator.h"

namespace vorb {
    namespace ecs {
        class ComponentTableBase;
        
        typedef std::pair<nString, ComponentTableBase*> NamedComponent; ///< A component table paired with its name
        typedef std::unordered_map<nString, TableID> ComponentSet; ///< Mapping of names to component table IDs
        typedef std::vector<ComponentTableBase*> ComponentList;

        /// Entity Component System
        class ECS {
            friend class ComponentTableBase;
        public:
            /// Default constructor which initializes events
            ECS();

            /// @return Set of entities for iteration
            const EntitySet& getEntities() const {
                return m_entities;
            }
            /// @return Number of entities that are active
            size_t getActiveEntityCount() const {
                return m_genEntity.getActiveCount();
            }
            /// @return The dictionary of NamedComponents
            const ComponentSet& getComponents() const {
                return m_components;
            }

            /// @return The ID of a newly generated entity
            EntityID addEntity();
            /// Delete an entity from this ECS
            /// @param id: The entity's ID
            /// @return True if an entity was deleted
            bool deleteEntity(EntityID id);
            /// Generate a chunk of entities
            /// @param n: Number of entities to generate
            /// @param ids: Pointer to output array of entities
            void genEntities(const size_t& n, EntityID* ids) {
                for (size_t i = 0; i < n; i++) ids[i] = addEntity();
            }

            /// Add a component to an entity
            /// @param name: Friendly name of component
            /// @param id: Component owner entity
            /// @return ID of generated component
            ComponentID addComponent(nString name, EntityID id);
            /// Remove a component from an entity
            /// @param name: Friendly name of component
            /// @param id: Component owner entity
            /// @return True if a component was deleted
            bool deleteComponent(nString name, EntityID id);
            /// Check if an entity has a component
            /// @param tableID: ID of the component table
            /// @param id: Entity
            /// @return True if the entity holds that component
            bool hasComponent(const TableID& tableID, const EntityID& id) const;
            /// Check if an entity has a component
            /// @param name: Friendly name of the component
            /// @param id: Entity
            /// @return True if the entity holds that component
            bool hasComponent(const nString& name, const EntityID& id) const;

            /// Add a component table to be referenced by a special name
            /// @param name: Friendly name of component table
            /// @param table: Component table
            TableID addComponentTable(nString name, ComponentTableBase* table);
            /// 
            /// @param name: Friendly name
            /// @return ID of the table, or 0 if none
            TableID getComponentTableID(const nString& name) const;
            /// Obtain a component table by its name
            /// @param name: Friendly name
            /// @return The component table
            ComponentTableBase* getComponentTable(nString name) const;
            /// Obtain a component table by its name
            /// @param id: Component table's ID
            /// @return The component table
            ComponentTableBase* getComponentTable(TableID id) const;

            Event<EntityID> onEntityAdded; ///< Called when an entity is added to this system
            Event<EntityID> onEntityRemoved; ///< Called when an entity is removed from this system
            Event<NamedComponent> onComponentAdded; ///< Called when a component table is added to this system
        private:
            typedef std::pair<ComponentTableBase*, std::shared_ptr<Delegate<Sender, EntityID>>> ComponentSubscriber;
            typedef std::unordered_map<nString, ComponentSubscriber> ComponentSubscriberSet;

            EntitySet m_entities; ///< List of entities
            EntityID m_eidHighest = 0; ///< Highest generated entity ID
            BitTable m_entityComponents; ///< Truth table for components that an entity holds

            vcore::IDGenerator<EntityID> m_genEntity; ///< Unique ID generator for entities
            ComponentSet m_components; ///< List of component tables
            ComponentList m_componentList; ///< Component tables organized by their id
        };
    }
}
namespace vecs = vorb::ecs;

#endif // !Vorb_ECS_h__
