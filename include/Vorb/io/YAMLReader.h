//
// YAMLReader.h
// Vorb Engine
//
// Created by Cristian Zaloj on 21 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file YAMLReader.h
 * @brief API for reading YAML data.
 */

#pragma once

#ifndef Vorb_YAMLReader_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAMLReader_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <unordered_set>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "../PtrRecycler.hpp"
#include "../Events.hpp"
#include "YAMLNode.h"

namespace keg {
    /// Reads YAML data
    class YAMLReader {
    public:
        /// Point this reader to a string of data
        /// @param data: Data string in YAML format
        void init(const cString data);
        /// Destroy all nodes created by this document (all nodes are now invalid)
        void dispose();

        /// @return The document's top-level node
        CALLEE_DELETE Node getFirst() const {
            return m_first;
        }
        /// Retrieve an interior node
        /// @pre: node must not be null
        /// @param node: Node
        /// @param value: Name of interior node
        /// @return The interior node, if it exists
        CALLEE_DELETE Node getInterior(Node node, const cString value);
        /// Retrieve an interior node
        /// @pre: node must not be null
        /// @param node: Node
        /// @param value: Name of interior node
        /// @return The interior node, if it exists
        CALLEE_DELETE Node getInterior(Node node, const nString& value) {
            return getInterior(node, value.c_str());
        }
        /// Deallocates this node (invalidates it and its copies)
        /// @param node: Node which will become null after being freed
        void free(Node& node);

        /// Performs an operation on each node in a mapped node
        /// @param node: Node map
        /// @param f: Function to be invoked on each element - (key, value) -> void
        void forAllInMap(Node node, Delegate<Sender, const nString&, Node>* f);
        /// Performs an operation on each node in a node array
        /// @param node: Node array
        /// @param f: Function to be invoked on each element - (index, value) -> void
        void forAllInSequence(Node node, Delegate<Sender, size_t, Node>* f);
    private:
        Node m_first; ///< The root node in the data
        std::unordered_set<Node> m_allocated; ///< List of allocated nodes
    };
}

#endif // !Vorb_YAMLReader_h__
