//
// YAMLNode.h
// Vorb Engine
//
// Created by Cristian Zaloj on 21 Jan 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file YAMLNode.h
 * 
 * \brief YAML node type and utilities.
 */

#pragma once

#ifndef Vorb_YAMLNode_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAMLNode_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace keg {
    struct YAMLNode; ///< A node in a YAML document (hidden)
    typedef YAMLNode* Node; ///< A usable YAML node
    template<typename T> struct NodeValueConverter; ///< Converts nodes to and from values

    class YAMLEmitter; ///< A YAML node emitter implementation

    /// Node states
    enum class NodeType {
        NONE, ///< This is a bad or empty node
        VALUE, ///< This node is a leaf
        MAP, ///< This node contains mapped nodes
        SEQUENCE ///< This node acts as an array of nodes
    };

    /// Determine the usage for a node
    /// \pre: node must not be null
    /// \param node: Node
    /// \return Node's type
    NodeType getType(Node node);
    /// Check if a node has an interior node
    /// \pre: node must not be null
    /// \param node: Node
    /// \param value: Name of interior node
    /// \return True if interior exists
    bool hasInterior(Node node, const cString value);
    /// Check if a node has an interior node
    /// \pre: node must not be null
    /// \param node: Node
    /// \param value: Name of interior node
    /// \return True if interior exists
    inline bool hasInterior(Node node, const nString& value) {
        return hasInterior(node, value.c_str());
    }
    /// Determine the array size of a node
    /// \pre: node must not be null
    /// \param node: Node
    /// \return Size of the array this node contains
    size_t getSequenceSize(Node node);

    /// Convert this node into a more useful value
    /// \pre: node must not be null
    /// \tparam T: Type to be retrieved
    /// \param node: Node
    /// \return Node's value as the specified type
    template<typename T>
    T convert(Node node) {
        return NodeValueConverter<T>::convert(node);
    }
}

#endif // !Vorb_YAMLNode_h__
