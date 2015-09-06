template <typename T>
inline void IntervalTree<typename T>::initSingle(T data, size_t length) {
    m_root = 0;
    m_tree.emplace_back(data, 0, length);
    m_tree[0].paintBlack();
}

template <typename T>
void IntervalTree<typename T>::initFromSortedArray(const std::vector <LNode>& data) {
    m_tree.resize(data.size());
    for (size_t i = 0; i < m_tree.size(); i++) {
        m_tree[i].setStart(data[i].start);
        m_tree[i].length = data[i].length;
        m_tree[i].data = data[i].data;
    }
    m_root = arrayToRedBlackTree(0, data.size() - 1, -1, true);
}

template <typename T>
inline void IntervalTree<typename T>::initFromSortedArray(const LNode data[], size_t size) {
    m_tree.resize(size);
    for (size_t i = 0; i < size; i++) {
        m_tree[i].setStart(data[i].start);
        m_tree[i].length = data[i].length;
        m_tree[i].data = data[i].data;
    }
    m_root = arrayToRedBlackTree(0, size - 1, -1, true);
}

template <typename T>
inline void IntervalTree<typename T>::clear() {
    std::vector<Node>().swap(m_tree);
    std::vector<NodeToAdd>().swap(m_nodesToAdd);
    std::vector<ui16>().swap(m_nodesToRemove);
    m_root = -1;
}

template <typename T>
inline const T& IntervalTree<typename T>::getData(size_t index) const {
    return m_tree[getInterval(index)].data;
}

//Get the enclosing interval for a given point
template <typename T>
i16 IntervalTree<typename T>::getInterval(size_t index) const {
    i32 interval = m_root;
    while (true) {

        // TODO(Ben): Error checking. Ideally we can remove this when we are certain this condition can never happen
        if (interval < 0 || interval >= (i32)m_tree.size()) {
            std::cout << "getInterval failed! Looking for index: " << index << " Interval is " << interval << std::endl;
            int a;
            std::cin >> a;
        }

        const Node& node = m_tree[interval];

        if (index < node.getStart()) { //check for go left
            interval = node.left;
        } else if (index < node.getStart() + node.length) { //check for at interval
            return interval;
        } else { //go right
            interval = node.right;
        }
    }
}

template <typename T>
bool IntervalTree<typename T>::treeInsert(int index, T data, int &newIndex) {
    int interval = m_root;
    Node* enclosingInterval = nullptr;
    int enclosingIndex = -1;
    while (true) {
        Node& node = m_tree[interval];

        if (index < (int)node.getStart()) { //go left

            //Check if we are at the leaf
            if (node.left == -1) {
                //check if we are right before the current node               
                if (index == node.getStart() - 1) {

                    if (enclosingInterval) {
                        --(enclosingInterval->length);
                    }

                    //If data is the same, simply combine with the node
                    if (data == node.data) {
                        node.decrementStart();
                        ++(node.length);

                        newIndex = interval;

                        return false;
                    }

                    node.left = m_tree.size();
                    newIndex = node.left;
                    m_tree.emplace_back(data, index, 1);
                    m_tree.back().parent = interval;

                    return true;
                }
                //If we get here, we are splitting an interval

                node.left = m_tree.size();
                newIndex = node.left;

                //is this check even necesarry?
                if (enclosingInterval) {
                    enclosingInterval->length = index - enclosingInterval->getStart();

                    //We need to add a new node to the tree later
                    m_nodesToAdd.emplace_back(index + 1, node.getStart() - index - 1, enclosingInterval->data);
                }

                m_tree.emplace_back(data, index, 1);
                m_tree.back().parent = interval;
                return true;
            }
            interval = node.left;
        } else if (index < node.getStart() + node.length) { //we are in the nodes interval

            if (node.data == data) {
                newIndex = interval;
                return false;
            } else if (node.getStart() == index) { //check if we are at the start of the interval
                //check for interval replacement
                if (node.length == 1) {
                    node.data = data;
                    newIndex = interval;
                    return false;
                }

                //move the interval over to make room for the new node
                node.incrementStart();
                --(node.length);

                //Check for if we are at leaf
                if (node.left == -1) {
                    node.left = m_tree.size();
                    newIndex = node.left;
                    m_tree.emplace_back(data, index, 1);
                    m_tree.back().parent = interval;
                    return true;
                }

                //If we get here, then we must continue along to the left to do the proper insertion
                interval = node.left;
            } else {
                enclosingIndex = interval;
                enclosingInterval = &node;
                //go right
                //Check if we are at the leaf
                if (node.right == -1) {

                    node.right = m_tree.size();
                    newIndex = node.right;

                    if (index == node.getStart() + node.length - 1) { //at the edge of the interval
                        --(enclosingInterval->length);
                    } else { //splitting the interval
                        m_nodesToAdd.emplace_back(index + 1, node.getStart() + node.length - index - 1, enclosingInterval->data);
                        enclosingInterval->length = index - enclosingInterval->getStart();
                    }

                    m_tree.emplace_back(data, index, 1);
                    m_tree.back().parent = interval;
                    return true;
                }
                interval = node.right;
            }
        } else { //go right
            //Check if we are at the leaf
            if (node.right == -1) {
                node.right = m_tree.size();
                newIndex = node.right;
                //is this check even necessary?
                if (enclosingInterval) {
                    enclosingInterval->length = index - enclosingInterval->getStart();
                    //We need to add a new node to the tree later
                    m_nodesToAdd.emplace_back(index + 1, node.getStart() - index - 1, enclosingInterval->data);
                }

                m_tree.emplace_back(data, index, 1);
                m_tree.back().parent = interval;
                return true;
            }
            interval = node.right;
        }
    }
}

template <typename T>
inline int IntervalTree<typename T>::getGrandparent(Node* node) {
    if (node->parent != -1) {
        return m_tree[node->parent].parent;
    } else {
        return -1;
    }
}

template <typename T>
inline int IntervalTree<typename T>::getUncle(Node* node, Node** grandParent) {
    int grandparentIndex = getGrandparent(node);
    if (grandparentIndex == -1) {
        *grandParent = nullptr;
        return -1; // No grandparent means no uncle
    }

    Node* g = &m_tree[grandparentIndex];
    *grandParent = g;

    if (node->parent == g->left) {
        return g->right;
    } else {
        return g->left;
    }
}

template <typename T>
inline void IntervalTree<typename T>::rotateParentLeft(int index, Node* grandParent) {
    Node& node = m_tree[index];
    i16 parentIndex = node.parent;
    Node& parent = m_tree[parentIndex];

    node.parent = parent.parent;
    parent.parent = index;
    parent.right = node.left;

    grandParent->left = index;
    if (node.left != -1) {
        m_tree[node.left].parent = parentIndex;
    }
    node.left = parentIndex;

}

template <typename T>
inline void IntervalTree<typename T>::rotateParentRight(int index, Node* grandParent) {
    Node& node = m_tree[index];
    i16 parentIndex = node.parent;
    Node& parent = m_tree[parentIndex];

    node.parent = parent.parent;
    parent.parent = index;
    parent.left = node.right;

    grandParent->right = index;
    if (node.right != -1) {
        m_tree[node.right].parent = parentIndex;
    }
    node.right = parentIndex;
}

template <typename T>
inline void IntervalTree<typename T>::rotateRight(int index) {

    Node& node = m_tree.at(index);
    Node& left = m_tree.at(node.left);

    i16 right = left.right;
    left.right = index;
    left.parent = node.parent;

    if (node.parent != -1) {
        Node& parent = m_tree.at(node.parent);
        if (parent.left == index) {
            parent.left = node.left;
        } else {
            parent.right = node.left;
        }
    }
    node.parent = node.left;

    node.left = right;
    if (right != -1) {
        m_tree[right].parent = index;
    }

}

template <typename T>
inline void IntervalTree<typename T>::rotateLeft(int index) {

    Node& node = m_tree.at(index);
    Node& right = m_tree.at(node.right);

    i16 left = right.left;
    right.left = index;
    right.parent = node.parent;

    if (node.parent != -1) {
        Node& parent = m_tree.at(node.parent);
        if (parent.right == index) {
            parent.right = node.right;
        } else {
            parent.left = node.right;
        }
    }
    node.parent = node.right;

    node.right = left;
    if (left != -1) {
        m_tree[left].parent = index;
    }
}

template <typename T>
void IntervalTree<typename T>::uncompressTraversal(int index, int& bufferIndex, T* buffer) {
    if (m_tree[index].left != -1) {
        uncompressTraversal(m_tree[index].left, bufferIndex, buffer);
    }
    for (int i = 0; i < m_tree[index].length; i++) {
        buffer[bufferIndex++] = m_tree[index].data;
    }
    if (m_tree[index].right != -1) {
        uncompressTraversal(m_tree[index].right, bufferIndex, buffer);
    }
}

template <typename T>
void IntervalTree<typename T>::uncompressIntoBuffer(T* buffer) {
    int bufferIndex = 0;
    uncompressTraversal(m_root, bufferIndex, buffer);
}

template <typename T>
typename IntervalTree<typename T>::Node* IntervalTree<typename T>::insert(size_t index, T data) {

    int nodeIndex;
    if (!treeInsert(index, data, nodeIndex)) {
        return &m_tree[nodeIndex];
    }

    Node* grandparent;
    //Balance the tree
    int newIndex = nodeIndex;
    while (true) {
        Node* node = &m_tree[newIndex];

        //Case 1: Current node is root
        if (node->parent == -1) {
            node->paintBlack();
            break;
        }

        //Case 2: Parent is black.
        if (m_tree[node->parent].isRed() == false) {
            break;
        }
        //Case 3: Both parent and uncle are red
        int uncleIndex = getUncle(node, &grandparent);

        if (uncleIndex != -1) {
            Node& uncle = m_tree[uncleIndex];
            if (uncle.isRed()) {
                m_tree[node->parent].paintBlack();
                uncle.paintBlack();
                grandparent->paintRed();
                newIndex = uncle.parent; //now we operate on the grandparent and start over
                continue;
            }

        }


        int dbg = 4;
        int npar = node->parent;
        int nl = node->left;
        int nr = node->right;
        //Case 4: Parent is red, uncle is black.
        if (newIndex == m_tree[node->parent].right && node->parent == grandparent->left) {
            rotateParentLeft(newIndex, grandparent);

            newIndex = node->left;

            node = &m_tree[newIndex];

            grandparent = &m_tree[m_tree[node->parent].parent];

        } else if (newIndex == m_tree[node->parent].left && node->parent == grandparent->right) {
            rotateParentRight(newIndex, grandparent);

            newIndex = node->right;

            node = &m_tree[newIndex];

            grandparent = &m_tree[m_tree[node->parent].parent];
        }

        //Case 5 Parent is red, Uncle is black.
        m_tree[node->parent].paintBlack();
        grandparent->paintRed();
        if (newIndex == m_tree[node->parent].left) {
            //Check for root change
            if (m_tree[node->parent].parent == m_root) m_root = node->parent;

            rotateRight(m_tree[node->parent].parent);

        } else {
            //Check for root change
            if (m_tree[node->parent].parent == m_root) m_root = node->parent;
            rotateLeft(m_tree[node->parent].parent);
        }

        break;
    }

    while (m_nodesToAdd.size()) {
        NodeToAdd n = m_nodesToAdd.back();
        m_nodesToAdd.pop_back();

        if (n.length) {
            Node* nn = insert(n.start, n.data);
            if (n.length > 1) {
                nn->length = n.length;
            }
        }
    }

    return &m_tree[nodeIndex];
}

// Iterators
template <typename T>
IntervalTree<typename T>::iterator::iterator(pointer ptr, std::vector <Node>* tree) : m_ptr(ptr), m_tree(tree) {
    if (m_ptr == nullptr) return;
    while (m_ptr->left != -1) m_ptr = &m_tree->operator[](m_ptr->left);
}

template <typename T>
typename IntervalTree<typename T>::iterator::self_type IntervalTree<typename T>::iterator::operator++() {
    if (m_ptr == nullptr) throw std::runtime_error("Attempted to increment iterator at end.");
    self_type i = *this;
    pointer r = m_ptr;
    // if you can walk right, walk right, then fully left.
    // otherwise, walk up until you come from left.
    if (m_ptr->right != -1) {
        m_ptr = &m_tree->operator[](m_ptr->right);
        while (m_ptr->left != -1)
            m_ptr = &m_tree->operator[](m_ptr->left);
        return self_type(r, m_tree);
    } else while (true) {
        if (m_ptr->parent == -1) {
            m_ptr = nullptr;
            return self_type(r, m_tree);
        }
        Node* p = &m_tree->operator[](m_ptr->parent);
        if (&m_tree->operator[](p->left) == m_ptr) {
            m_ptr = p;
            return self_type(r, m_tree);
        }
        m_ptr = &m_tree->operator[](m_ptr->parent);
    }
    return i;
}

template <typename T>
IntervalTree<typename T>::const_iterator::const_iterator(pointer ptr, std::vector <Node>* tree) : m_ptr(ptr), m_tree(tree) {
    if (m_ptr == nullptr) return;
    while (m_ptr->left != -1) m_ptr = &m_tree->operator[](m_ptr->left);
}

template <typename T>
typename IntervalTree<typename T>::const_iterator::self_type IntervalTree<typename T>::const_iterator::operator++() {
    if (m_ptr == nullptr) throw std::runtime_error("Attempted to increment const_iterator at end.");
    self_type i = *this;
    pointer r = m_ptr;
    // if you can walk right, walk right, then fully left.
    // otherwise, walk up until you come from left.
    if (m_ptr->right != -1) {
        m_ptr = &m_tree->operator[](m_ptr->right);
        while (m_ptr->left != -1)
            m_ptr = &m_tree->operator[](m_ptr->left);
        return self_type(r, m_tree);
    } else while (true) {
        if (m_ptr->parent == -1) {
            m_ptr = nullptr;
            return self_type(r, m_tree);
        }
        Node* p = &m_tree->operator[](m_ptr->parent);
        if (&m_tree->operator[](p->left) == m_ptr) {
            m_ptr = p;
            return self_type(r, m_tree);
        }
        m_ptr = &m_tree->operator[](m_ptr->parent);
    }
    return i;
}
