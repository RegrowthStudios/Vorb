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
    vorb::Quaternion<f32> q1 = vmath::angleAxis(3.1f, f32v3(0.3f, 0.5f, 0.2f));
    glm::tquat<f32> q2 = glm::angleAxis(3.1f, glm::vec3(0.3f, 0.5f, 0.2f));
    if (memcmp(&q1, &q2, sizeof(q1)) != 0) return false;
    if (memcmp(&vmath::mat4_cast(q1), &glm::mat4_cast(q2), sizeof(f32m4)) != 0) return false;
    if (vmath::roll(q1) != glm::roll(q2)) return false;
    if (vmath::pitch(q1) != glm::pitch(q2)) return false;
    if (vmath::yaw(q1) != glm::yaw(q2)) return false;
    q1 = vmath::rotate(q1, 1.12f, f32v3(0.2f, 0.4f, 0.5f));
    q2 = glm::rotate(q2, 1.12f, glm::vec3(0.2f, 0.4f, 0.5f));
    if (memcmp(&q1, &q2, sizeof(q1)) != 0) return false;
    if (glm::vec3(vmath::eulerAngles(q1)) != glm::eulerAngles(q2)) return false;
    // W parameter is last in vorb
    q1 = vorb::Quaternion<f32>(1.0f, 2.0f, 3.0f, 4.0f);
    q2 = glm::tquat<f32>(4.0f, 1.0f, 2.0f, 3.0f);
    if (memcmp(&q1, &q2, sizeof(q1)) != 0) return false;
    q1 = vorb::Quaternion<f32>(f32v3(2.0f, 1.0f, 2.3f));
    q2 = glm::tquat<f32>(glm::vec3(2.0f, 1.0f, 2.3f));
    if (memcmp(&q1, &q2, sizeof(q1)) != 0) return false;
    f32v3 v1;
    glm::vec3 v2;
    v1 = f32v3(1.0f, 2.0f, 3.0f) * q1;
    v2 = glm::vec3(1.0f, 2.0f, 3.0f) * q2;
    if (glm::vec3(v1) != v2) return false;
    v1 = q1 * f32v3(1.0f, 2.0f, 3.0f);
    v2 = q2 * glm::vec3(1.0f, 2.0f, 3.0f);
    if (glm::vec3(v1) != v2) return false;
    if (memcmp(&vmath::normalize(q1), &glm::normalize(q2), sizeof(q1)) != 0) return false;
    return true;
}
