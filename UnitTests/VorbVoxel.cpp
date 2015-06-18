#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Voxel_

#include <include/voxel/IntervalTree.h>
#include <include/Vorb.h>

TEST(IntervalTree) {
    
    IntervalTree<ui16> tree;
    tree.initSingle(0, 32768);

    assert(tree[0].data == 0);
    for (int i = 0; i < 32768; i++) {
        tree.insert(i, i);
    }
    for (int i = 0; i < 32768; i++) {

        if (tree[i].data != i) {
            std::cout << tree[i].data << " != " << i << std::endl;
            return false;
        }
        assert(tree[i].length == 1);
    }

    return true;
}