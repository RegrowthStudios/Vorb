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
    int i = 0;
    for (auto& it : tree) {
        if (it.data != i) {
            std::cout << it.data << " != " << i << std::endl;
            return false;
        }
        assert(it.length == 1);
        i++;
    }

    return true;
}