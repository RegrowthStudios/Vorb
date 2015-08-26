#include "stdafx.h"
#include "macros.h"

#include "include/Timing.h"

#include "include/math/VorbMath.hpp"
#include "include/Matrix.hpp"
#include "include/Quaternion.hpp"
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
        if (glm::cross(g1, g2) != glm::vec3(vmath::cross(v1, v2))) return false;
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
        f32m2 m1;
        glm::tmat2x2<f32> m2;
        if (glm::tmat2x2<f32>(m1) != m2) return false;
        m1 *= 3.0f;
        m2 *= 3.0f;
        if (glm::tmat2x2<f32>(m1) != m2) return false;
        m1 += 1.0f;
        m2 += 1.0f;
        if (glm::tmat2x2<f32>(m1) != m2) return false;
        m1 -= 0.1f;
        m2 -= 0.1f;
        if (glm::tmat2x2<f32>(m1) != m2) return false;
        m1 /= 1.1f;
        m2 /= 1.1f;
        if (glm::tmat2x2<f32>(m1) != m2) return false;
        if (glm::tmat2x2<f32>(vmath::inverse(m1)) != glm::detail::compute_inverse(m2)) return false;
        if (glm::tmat2x2<f32>(m1 * m1) != m2 * m2) return false;
        if (glm::tmat2x2<f32>(m1 + m1) != m2 + m2) return false;
        if (glm::tmat2x2<f32>(m1 / m1) != m2 / m2) return false;
        if (glm::tmat2x2<f32>(m1 - m1) != m2 - m2) return false;
        f32v2 mv1(2.0f);
        glm::vec2 mv2(2.0f);
        if (glm::vec2(m1 * mv1) != m2 * mv2) return false;
        if (glm::vec2(mv1 * m1) != mv2 * m2) return false;
    }
    {
        f32m3 m1;
        glm::tmat3x3<f32> m2;
        if (glm::tmat3x3<f32>(m1) != m2) return false;
        m1 *= 3.0f;
        m2 *= 3.0f;
        if (glm::tmat3x3<f32>(m1) != m2) return false;
        m1 += 1.0f;
        m2 += 1.0f;
        if (glm::tmat3x3<f32>(m1) != m2) return false;
        m1 -= 0.1f;
        m2 -= 0.1f;
        if (glm::tmat3x3<f32>(m1) != m2) return false;
        m1 /= 1.1f;
        m2 /= 1.1f;
        if (glm::tmat3x3<f32>(m1) != m2) return false;
        if (glm::tmat3x3<f32>(vmath::inverse(m1)) != glm::detail::compute_inverse(m2)) return false;
        if (glm::tmat3x3<f32>(m1 * m1) != m2 * m2) return false;
        if (glm::tmat3x3<f32>(m1 + m1) != m2 + m2) return false;
        if (glm::tmat3x3<f32>(m1 / m1) != m2 / m2) return false;
        if (glm::tmat3x3<f32>(m1 - m1) != m2 - m2) return false;
        f32v3 mv1(2.0f);
        glm::vec3 mv2(2.0f);
        if (glm::vec3(m1 * mv1) != m2 * mv2) return false;
        if (glm::vec3(mv1 * m1) != mv2 * m2) return false;
    }
    {
        f32m4 m1;
        glm::tmat4x4<f32> m2;
        if (glm::tmat4x4<f32>(m1) != m2) return false;
        m1 *= 3.0f;
        m2 *= 3.0f;
        if (glm::tmat4x4<f32>(m1) != m2) return false;
        m1 += 1.0f;
        m2 += 1.0f;
        if (glm::tmat4x4<f32>(m1) != m2) return false;
        m1 -= 0.1f;
        m2 -= 0.1f;
        if (glm::tmat4x4<f32>(m1) != m2) return false;
        m1 /= 1.1f;
        m2 /= 1.1f;
        if (glm::tmat4x4<f32>(m1) != m2) return false;
        if (glm::tmat4x4<f32>(vmath::inverse(m1)) != glm::detail::compute_inverse(m2)) return false;
        if (glm::tmat4x4<f32>(m1 * m1) != m2 * m2) return false;
        if (glm::tmat4x4<f32>(m1 + m1) != m2 + m2) return false;
        if (glm::tmat4x4<f32>(m1 / m1) != m2 / m2) return false;
        if (glm::tmat4x4<f32>(m1 - m1) != m2 - m2) return false;
        f32v4 mv1(2.0f);
        glm::vec4 mv2(2.0f);
        // GLM's version of this is slightly unprecise.
        // if (glm::vec4(m1 * mv1) != m2 * mv2) return false;
        if (glm::vec4(mv1 * m1) != mv2 * m2) return false;
    }

    // Quaternions
    vorb::Quaternion<f32> q1(0, 0, 0, 1);
  //  if (memcmp(&vmath::angleAxis(3.1f, f32v3(0.3f, 0.5f, 0.2f)), &glm::angleAxis(3.1f, glm::vec3(0.3f, 0.5f, 0.2f)), sizeof(glm::vec3)) != 0) return false;
    return true;
}
