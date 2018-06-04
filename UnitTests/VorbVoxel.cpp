#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Voxel_

#include <include/voxel/IntervalTree.h>
#include <include/Vorb.h>
#include <include/Timing.h>

TEST(IntervalTree) {
    
    // Ordered insertion
    IntervalTree<ui16> tree;
    tree.initSingle(0, 32768);

    test_assert(tree[0].data == 0);
    for (int i = 0; i < 32768; i++) {
        tree.insert(i, i);
    }
    int i = 0;
    for (auto& it : tree) {
        if (it.data != i) {
            std::cout << it.data << " != " << i << std::endl;
            return false;
        }
        test_assert(it.length == 1);
        i++;
    }

    if (!tree.checkTreeValidity()) throw 333;

    std::cout << tree.size() << std::endl;
    tree.clear();
    // Random insertions, big range
    tree.initSingle(0, 32768);

    test_assert(tree[0].data == 0);
    for (int i = 0; i < 1000000; i++) {
        tree.insert(rand() % 32768, rand() % 60000);
    }

    if (!tree.checkTreeValidity()) throw 333;
    std::cout << tree.size() << std::endl;

    // Time lookups
    const int LOOKUPS = 100000;
    int a = 0;
    PreciseTimer timer;
    for (int i = 0; i < LOOKUPS; i++) {
        a += tree.getData(i % 32768);
    }
    printf("Avg lookup time %lf ms\n", timer.stop() / LOOKUPS);
    std::cout << a << std::endl;
    tree.clear();

    // Random insertions, small range
    tree.initSingle(0, 32768);

    test_assert(tree[0].data == 0);
    for (int i = 0; i < 1000000; i++) {
        tree.insert(rand() % 32768, rand() % 8);
    }

    if (!tree.checkTreeValidity()) throw 333;
    std::cout << tree.size() << std::endl;
    tree.clear();

    // Random insertions, very small range
    tree.initSingle(0, 32768);
    test_assert(tree[0].data == 0);
    for (int i = 0; i < 1000000; i++) {
        tree.insert(rand() % 32768, rand() % 3);
    }

    std::cout << tree.size() << std::endl;
    return true;
}