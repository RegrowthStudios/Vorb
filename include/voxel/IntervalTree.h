//
// IntervalTree.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IntervalTree.h
* @brief Interval tree data structure for storing
* RLE compressed data in a contiguous buffer with
* O(log(n)) lookup and insertion speed.
*/

#pragma once

#ifndef Vorb_IntervalTree_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IntervalTree_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#pragma once
#include <map>

//Implementation of a specialized interval tree based on a red-black tree
//Red black tree: http://en.wikipedia.org/wiki/Red%E2%80%93black_tree

// NOTE: Currently only works for lengths 32768 and below // TODO(Ben): More templatey
template <typename T>
class IntervalTree {
public:

#define COLOR_BIT 0x8000
#define START_MASK 0x7FFF

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
        Node() {}
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

    void initSingle(T data, size_t length);
    void initFromSortedArray(const std::vector <LNode>& data);
    void initFromSortedArray(LNode data[], size_t size);

    bool checkTreeValidity() const {
        int tot = 0;
        for (size_t i = 0; i < m_tree.size(); i++) {
            if (m_tree[i].length > CHUNK_SIZE) {
                std::cout << "AHA";
                fflush(stdout);
            }
            tot += m_tree[i].length;
        }
        if (tot != CHUNK_SIZE) {
            pError("Invalid Tree! Total size is not 32768, it is " + std::to_string(tot));
            fflush(stdout);
        }

        if (m_tree[m_root].isRed()) {
            std::cout << "ROOT IS RED!\n";
            fflush(stdout);
        }
        checkValidRB();
        return true;
    }

    bool checkValidRB() const {
        if (m_tree[m_root].parent != -1) {
            std::cout << "ROOT HAS A PARENT!\n";
            fflush(stdout);
        }
        for (int i = 0; i < size(); i++) {
            if (m_tree[i].parent == i) {
                std::cout << "Node is own parent!";
            }
            if (m_tree[i].parent == i) {
                std::cout << "Node is own parent!";
            }
            if (m_tree[i].left == i) {
                std::cout << "Node is own left!";
            }
            if (m_tree[i].right == i) {
                std::cout << "Node is own right!";
            }
            if (m_tree[i].parent != -1) {
                if (m_tree[m_tree[i].parent].left != i && m_tree[m_tree[i].parent].right != i) {
                    std::cout << "Node is disconnected from parent!\n";
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

    inline Node& operator[](int index) { return m_tree[index]; }
    inline int size() const { return m_tree.size(); }

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
