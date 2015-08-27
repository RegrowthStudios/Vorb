#include "stdafx.h"
#include "macros.h"

#include "include/Timing.h"

#include "include/Matrix.hpp"
#include "include/Quaternion.hpp"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Math_

TEST(Utilities) {

    const int NUM_TESTS = 10000000;
    f32 tot = 0.0f;
    PreciseTimer timer;
    timer.start();
    for (int i = 0; i < NUM_TESTS; i++) {
        f32 val = rand() % RAND_MAX / (f32)RAND_MAX;

        tot += vmath::floor(val);
        tot += vmath::ceil(val);
    }
    printf("%lf ms\n", timer.stop());
    std::cout << tot;


    // TODO(Ben): Good unit testing!
    return false;
    return true;
}
