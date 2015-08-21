#include "stdafx.h"
#include "macros.h"

#include "include/VorbMath.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Math_

TEST(Utilities) {
    af32v3 g1(253.0f, 299632.0f, 42356.0f);
    af32v3 g2(53626.0f, 24632.0f, 2361.0f);
    f32v3 v1(253.0f, 299632.0f, 42356.0f);
    f32v3 v2(53626.0f, 24632.0f, 2361.0f);
    if (glm::length(g1) != vmath::length(v1)) return false;
    if (glm::cross(g1, g2) != vmath::cross(v1, v2).getAligned()) return false;
    if (glm::dot(g1, g2) != vmath::dot(v1, v2)) return false;
    if (glm::normalize(g1) != vmath::normalize(v1).getAligned()) return false;
    return true;
}
