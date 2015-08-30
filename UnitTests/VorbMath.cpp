#include "stdafx.h"
#include "macros.h"

#include "include/Timing.h"

#include "include/Matrix.hpp"
#include "include/Quaternion.hpp"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Math_

TEST(Utilities) {

    for (f64 i = 0; i < M_2_PI; i += 0.1) {
        f64q qq(f64v3(0.0, i, 0.0));
        f64v3 e = vmath::eulerAngles(qq);
        std::cout << i << " " << e.x << " " << e.y << " " << e.z << std::endl;
    }

    // TODO(Ben): Good unit testing!
    return false;
    return true;
}
