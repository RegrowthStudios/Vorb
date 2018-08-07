//
// IntervalTree.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IntervalTree.h
 * @brief Interval tree data structure for storing RLE compressed data in a contiguous buffer with O(log(n)) lookup and insertion speed.
 */

#pragma once

#ifndef Vorb_IntervalTree_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IntervalTree_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>

#include "../types.h"
#endif // !VORB_USING_PCH

// Implementation of a specialized interval tree based on a red-black tree
// Red black tree: http://en.wikipedia.org/wiki/Red%E2%80%93black_tree

// NOTE: Currently only works for lengths 32768 and below
// TODO(Ben): More templatey
// TODO(Ben): Recombination
// TODO(Ben): Refactor
template <typename T>
class IntervalTree {
public:

#define COLOR_BIT 0x8000u
#define START_MASK 0x7FFFu

    // Lightweight node for initialization
    class LNode {
    public:
        LNode() {}
        LNode(ui16 Start, ui16 Length, T Data) : start(Start), length(Length), data(Data) {}
        void set(ui16 Start, ui16 Length, T Data) {
            start = Start;
            length = Length;
            data = Data;
        }
        ui16 start;
        ui16 length;
        T data;
    };

    class Node {
    public:
        Node() : left(-1), right(-1), parent(-2) {}
        Node(T Data, ui16 start, ui16 Length) : data(Data), m_start(start | COLOR_BIT), length(Length), left(-1), right(-1), parent(-1) {}

        inline void incrementStart() { ++m_start; }
        inline void decrementStart() { --m_start; }
        inline ui16 getStart() const { return m_start & START_MASK; }
        inline void setStart(ui16 Start) { m_start = (m_start & COLOR_BIT) | Start; }
        inline void paintRed() { m_start |= COLOR_BIT; }
        inline void paintBlack() { m_start &= START_MASK; }
        inline bool isRed() const { return (m_start & COLOR_BIT) != 0; }

        ui16 length;
        i16 left;
        i16 right;
        i16 parent;
    private:
        ui16 m_start; //also stores color
    public:
        T data;
    };

    class iterator {
    public:
        typedef iterator self_type;
        typedef Node value_type;
        typedef Node& reference;
        typedef Node* pointer;
        iterator(pointer ptr, std::vector <Node>* tree);
        self_type operator++();
        reference operator*() { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
        bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
    private:
        pointer m_ptr;
        std::vector <Node>* m_tree;
    };
    class const_iterator {
    public:
        typedef const_iterator self_type;
        typedef Node value_type;
        typedef Node& reference;
        typedef Node* pointer;
        const_iterator(pointer ptr, std::vector <Node>* tree);
        self_type operator++();
        const Node &operator*() { return *m_ptr; }
        const Node *operator->() { return m_ptr; }
        bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
        bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
    private:
        pointer m_ptr;
        std::vector <Node>* m_tree;
    };

    void initSingle(T data, size_t length);
    void initFromSortedArray(const std::vector <LNode>& data);
    void initFromSortedArray(const LNode data[], size_t size);

    bool checkTreeValidity() const {
        int tot = 0;
        for (size_t i = 0; i < m_tree.size(); i++) {
            if (m_tree[i].length > 32768) {
                return false;
            }
            tot += m_tree[i].length;
        }
        if (tot != 32768) {
            return false;
        }

        if (m_tree[m_root].isRed()) {
            return false;
        }
        return checkValidRB();
    }

    bool checkValidRB() const {
        if (m_tree[m_root].parent != -1) {
            return false;
        }
        for (int i = 0; i < (int)size(); i++) {
            if (m_tree[i].parent == i) {
                return false;
            }
            if (m_tree[i].left == i) {
                return false;
            }
            if (m_tree[i].right == i) {
                return false;
            }
            if (m_tree[i].parent != -1) {
                if (m_tree[m_tree[i].parent].left != i && m_tree[m_tree[i].parent].right != i) {
                    return false;
                }
            }
        }
        return true;
    }

    void clear();

    const T& getData(size_t index) const;
    //Get the enclosing interval for a given point
    i16 getInterval(size_t index) const;

    Node* insert(size_t index, T data);

    void uncompressIntoBuffer(T* buffer);

    iterator begin() { 
        if (m_root == -1) return iterator(nullptr, nullptr);
        return iterator(&m_tree[m_root], &m_tree);
    }
    iterator end() { return iterator(nullptr, nullptr); }

    const_iterator begin() const {
        if (m_root == -1) return const_iterator(nullptr, nullptr);
        return iterator(&m_tree[m_root], &m_tree);
    }
    const_iterator end() const { return const_iterator(nullptr, nullptr); }

    inline const Node& operator[](int index) const { return m_tree[index]; }
    inline size_t size() const { return m_tree.size(); }

private:
    void uncompressTraversal(int index, int& bufferIndex, T* buffer);

    int arrayToRedBlackTree(int i, int j, int parent, bool isBlack) {
        if (i > j) return -1;

        int x = (j + i) / 2;

        Node& node = m_tree[x];
        node.parent = parent;

        if (isBlack) {
            node.paintBlack();
        } else {
            node.paintRed();
        }

        if (i < j) {
            node.left = arrayToRedBlackTree(i, x - 1, x, !isBlack);
            node.right = arrayToRedBlackTree(x + 1, j, x, !isBlack);
        } else {
            node.left = -1;
            node.right = -1;
        }
        return x;
    }

    bool treeInsert(int index, T data, int &newIndex);

    int getGrandparent(Node* node);

    int getUncle(Node* node, Node** grandParent);

    void rotateParentRight(int index, Node* grandParent);

    void rotateParentLeft(int index, Node* grandParent);

    void rotateRight(int index);

    void rotateLeft(int index);

    int m_root = -1;
    std::vector <Node> m_tree;

    class NodeToAdd {
    public:
        NodeToAdd(i16 Parent, ui16 Length, T Data) : start(Parent), length(Length), data(Data) {}
        i16 start;
        ui16 length;
        T data;
    };

    std::vector <NodeToAdd> m_nodesToAdd;
    std::vector <ui16> m_nodesToRemove;
};

#include "IntervalTree.inl"

#endif // !Vorb_IntervalTree_h__
