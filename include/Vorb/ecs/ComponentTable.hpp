//
// ComponentTable.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 10 Nov 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ComponentTable.hpp
 * @brief Common abstract implementation of ComponentTableBase for any component type.
 */

#pragma once

#ifndef Vorb_ComponentTable_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ComponentTable_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include <utility>
#include <vector>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "ComponentTableBase.h"

namespace vorb {
    namespace ecs {
        /// Component table that stores a specific component type
        template<typename T>
        class ComponentTable : public ComponentTableBase {
        public:
            typedef std::pair<EntityID, T> ComponentPairing; ///< A pairing of an entity to a component
            typedef std::vector<ComponentPairing> ComponentList; ///< List of components accessed by component IDs

            /// Constructor that requires a blank component for reference
            /// @param defaultData: Blank component data
            ComponentTable(const T& defaultData) : ComponentTableBase() {
                // Default data goes in the first slot
                _components.emplace_back(ID_GENERATOR_NULL_ID, defaultData);
            }
            /// Default constructor that uses component constructor for defaults
            ComponentTable() : ComponentTable(T()) {
                // Empty
            }
        
            /// Obtain a component from this table
            /// @param cID: Component ID
            /// @return Const component reference
            const T& get(const ComponentID& cID) const {
                return _components[cID].second;
            }
            /// Obtain a component from this table
            /// @param cID: Component ID
            /// @return Component reference
            T& get(const ComponentID& cID) {
                return _components[cID].second;
            }
            /// Obtain an entity's component from this table
            /// @param eID: Entity ID
            /// @return Const component reference
            const T& getFromEntity(const EntityID& eID) const {
                return get(getComponentID(eID));
            }
            /// Obtain an entity's component from this table
            /// @param eID: Entity ID
            /// @return Component reference
            T& getFromEntity(const EntityID& eID) {
                return get(getComponentID(eID));
            }

            /// @return The blank component data
            const T& getDefaultData() const {
                return _components[0].second;
            }

            /// @return Iterator to the first pair of (entity ID, T)
            typename ComponentList::iterator begin() {
                // + 1 to skip the default element
                return ++_components.begin();
            }
            /// @return Iterator to the end of component pairing list
            typename ComponentList::iterator end() {
                return _components.end();
            }
            /// @return Const iterator to the first pair of (entity ID, T)
            typename ComponentList::const_iterator cbegin() const {
                return ++_components.cbegin();
            }
            /// @return Const iterator to the end of component pairing list
            typename ComponentList::const_iterator cend() const {
                return _components.cend();
            }

            /// @return Reference to component list for iteration
            operator const ComponentList& () const {
                return _components;
            }

            /// @return size of internal component list
            size_t getComponentListSize() const { return _components.size(); }

        protected:
            virtual void addComponent(ComponentID cID, EntityID eID) override {
                if (cID >= _components.size()) _components.resize(cID + 1);
                _components[cID].first = eID;
                _components[cID].second = getDefaultData();
            }
            virtual void setComponent(ComponentID cID, EntityID eID) override {
                _components[cID].first = eID;
                _components[cID].second = getDefaultData();
            }

            virtual void initComponent(ComponentID cID VORB_UNUSED, EntityID eID VORB_UNUSED) override {
                // Empty
            }
            virtual void disposeComponent(ComponentID cID VORB_UNUSED, EntityID eID VORB_UNUSED) override {
                // Empty
            }

            ComponentList _components; ///< A list of (entity ID, Component)
        };
    }
}
namespace vecs = vorb::ecs;

#endif // !Vorb_ComponentTable_hpp__
