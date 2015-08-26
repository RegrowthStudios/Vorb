#include "stdafx.h"
#include "macros.h"

#include "include/Vector.hpp"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH System_

TEST(TypeSizes) {
#define SA(T, S) assert(sizeof(T) == S)
    SA(float, 4);
    SA(double, 8);
    SA(f32, 4);
    SA(f64, 8);
    SA(i32, 4);
    SA(i64, 8);
    SA(i8v3, 3);
    SA(i16v3, 6);
    SA(i32v3, 12);
    SA(i64v3, 24);
    SA(f32v3, 12);
    SA(f64v3, 24);
    return true;
}

TEST(TypeOperations) {
#define SA(T, S) assert(sizeof(T) == S)
    { // Vec2
        vorb::Vector2<f32> va(2.0f);
        vorb::Vector2<f32> vb(glm::tvec2<f32>(1.0f));
        va += vb;
        if (va.y != 3.0f) return false;
        va += -1.0f;
        if (va.x != 2.0f) return false;
        vorb::Vector2<f32> vc = vb / va;
        if (vc.x != 0.5f) return false;
        va = vc;
        if (va != vc) return false;
        if (!(va == vc)) return false;
        va += vorb::Vector2<i32>(1);
        if (va.x != 1.5f) return false;
    }
    { // Vec3
        vorb::Vector3<f32> va(2.0f);
        vorb::Vector3<f32> vb(glm::tvec3<f32>(1.0f));
        va += vb;
        if (va.y != 3.0f) return false;
        va += -1.0f;
        if (va.x != 2.0f) return false;
        vorb::Vector3<f32> vc = vb / va;
        if (vc.x != 0.5f) return false;
        va = vc;
        if (va != vc) return false;
        if (!(va == vc)) return false;
        va += vorb::Vector3<i32>(1);
        if (va.x != 1.5f) return false;
    }
    { // Vec4
        vorb::Vector4<f32> va(2.0f);
        vorb::Vector4<f32> vb(glm::tvec4<f32>(1.0f));
        va += vb;
        if (va.y != 3.0f) return false;
        va += -1.0f;
        if (va.x != 2.0f) return false;
        vorb::Vector4<f32> vc = vb / va;
        if (vc.x != 0.5f) return false;
        va = vc;
        if (va != vc) return false;
        if (!(va == vc)) return false;
        va += vorb::Vector4<i32>(1);
        if (va.x != 1.5f) return false;
    }
    return true;
}