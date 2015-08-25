#include "stdafx.h"
#include "macros.h"

#include "include/Timing.h"

#include "include/math/VorbMath.h"
#include "include/Matrix.h"
#include <glm/gtc/quaternion.hpp>

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Math_

TEST(Utilities) {
    // Vectors
    {
        af32v2 g1(253.0f, 299632.0f);
        af32v2 g2(53626.0f, 24632.0f);
        f32v2 v1(253.0f, 299632.0f);
        f32v2 v2(53626.0f, 24632.0f);
        if (glm::length(g1) != vmath::length(v1)) return false;
        if (glm::dot(g1, g2) != vmath::dot(v1, v2)) return false;
    }
    {
        af32v3 g1(253.0f, 299632.0f, 42356.0f);
        af32v3 g2(53626.0f, 24632.0f, 2361.0f);
        f32v3 v1(253.0f, 299632.0f, 42356.0f);
        f32v3 v2(53626.0f, 24632.0f, 2361.0f);
        if (glm::length(g1) != vmath::length(v1)) return false;
        if (glm::cross(g1, g2) != vmath::cross(v1, v2).getAligned()) return false;
        if (glm::dot(g1, g2) != vmath::dot(v1, v2)) return false;
    }
    {
        af32v4 g1(253.0f, 299632.0f, 42356.0f, 42356.0f);
        af32v4 g2(53626.0f, 24632.0f, 2361.0f, 42356.0f);
        f32v4 v1(253.0f, 299632.0f, 42356.0f, 42356.0f);
        f32v4 v2(53626.0f, 24632.0f, 2361.0f, 42356.0f);
        if (glm::length(g1) != vmath::length(v1)) return false;
        if (glm::dot(g1, g2) != vmath::dot(v1, v2)) return false;
    }
    // Matrices
    {
        // Check identity
        vorb::Matrix2<f32> m1;
        f32m2 m2;
        if (f32m2(m1) != m2) return false;
        m1 *= 3.0f;
        m2 *= 3.0f;
        if (f32m2(m1) != m2) return false;
        m1 += 1.0f;
        m2 += 1.0f;
        if (f32m2(m1) != m2) return false;
        m1 -= 0.1f;
        m2 -= 0.1f;
        if (f32m2(m1) != m2) return false;
        m1 /= 1.1f;
        m2 /= 1.1f;
        if (f32m2(m1) != m2) return false;
        if (f32m2(vmath::computeInverse(m1)) != glm::detail::compute_inverse(m2)) return false;
        if (f32m2(m1 * m1) != m2 * m2) return false;
        if (f32m2(m1 + m1) != m2 + m2) return false;
        if (f32m2(m1 / m1) != m2 / m2) return false;
        if (f32m2(m1 - m1) != m2 - m2) return false;
        f32v2 mv1(2.0f);
        glm::vec2 mv2(2.0f);
        if (glm::vec2(m1 * mv1) != m2 * mv2) return false;
        if (glm::vec2(mv1 * m1) != mv2 * m2) return false;
    }
    // UNIT TESTING ENDS HERE

    // PERFORMANCE BENCHMARKING STARTS HERE (Doesn't affect pass/fail)
    puts("Vector timings:");
    const int N_TESTS = 10000000;
    f32 total = 0.0f;
    PreciseTimer timer;
    timer.start();
    for (int i = 0; i < N_TESTS; i++) {
        af32v3 g1(253.0f, 299632.0f, 42356.0f);
        af32v3 g2(53626.0f, 24632.0f, 2361.0f);
        g1 = glm::cross(g1, g2);
        total += glm::length(g1);
    }
    printf("GLM   %lf ms\n", timer.stop());
    timer.start();
    for (int i = 0; i < N_TESTS; i++) {
        f32v3 g1(253.0f, 299632.0f, 42356.0f);
        f32v3 g2(53626.0f, 24632.0f, 2361.0f);
        g1 = vmath::cross(g1, g2);
        total += vmath::length(g1);
    }
    printf("VMATH %lf ms\n", timer.stop());
    std::cout << total;
    return true;
}
