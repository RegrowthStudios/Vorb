//
// Entity.h
// Vorb Engine
//
// Created by Cristian Zaloj on 9 Nov 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file Entity.h
 * @brief Basic ECS types.
 */

#pragma once

#ifndef Vorb_Entity_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Entity_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <unordered_set>
#include <unordered_map>

#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace ecs {
        typedef ui32 EntityID; ///< Numeric ID type for entities
        typedef ui32 ComponentID; ///< Numeric ID type for components
        typedef ui32 TableID; ///< Numeric ID type for component tables
        typedef std::pair<EntityID, ComponentID> ComponentBinding;  ///< Pairing of entities and components
        typedef std::unordered_map<EntityID, ComponentID> ComponentBindingSet; ///< Set of entity-component pairings

        /// Basically an ID in an ECS
        class Entity {
        public:
            /// It must not be nameless
            /// @param _id: Unique non-zero id
            Entity(const EntityID& _id) :
                id(_id) {
                // Empty
            }

            const EntityID id; ///< Unique ID

            bool operator== (const Entity& e) const {
                return id == e.id;
            }
        };

        typedef std::unordered_set<Entity> EntitySet; ///< A set of entities
        typedef std::unordered_set<EntityID> EntityIDSet; ///< A set of entity IDs
    }
}
namespace vecs = vorb::ecs;

/// Entity hash implementation
template<> struct std::hash<vecs::Entity> {
public:
    size_t operator() (const vecs::Entity& e) const {
        return _internal(e.id);
    }
private:
    std::hash<vecs::EntityID> _internal;
};

#endif // !Vorb_Entity_h__
