///
/// IntersectionUtils.inl
/// Vorb Engine
///
/// Created by Benjamin Arnold on 11 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Intersection functions
///

#pragma once

#ifndef IntersectionUtils_inl__
#define IntersectionUtils_inl__

/************************************************************************/
/* Intersection functions                                               */
/************************************************************************/

namespace IntersectionUtils {

    /// Sums the 3 components of an f32v3
    inline float vecsum(const f32v3& v) {
        return v.x + v.y + v.z;
    }

    /// Calculates intersection between a ray and a sphere
    /// @param raydir: Direction of ray
    /// @param rayorig: Origin of ray
    /// @param pos: Center of sphere
    /// @param rad: Radius of sphere
    /// @param hitpoint: returned hit location
    /// @param distance: returned distance of collision
    /// @param normal: returned normal of collision
    /// @return true on collision
    inline bool sphereIntersect(const f32v3& raydir, const f32v3& rayorig, const f32v3& pos,
                                const float& rad, OUT f32v3& hitpoint, OUT float& distance, OUT f32v3& normal) {

        float a = vecsum(raydir*raydir);
        float b = vecsum(raydir * (2.0f * (rayorig - pos)));
        float c = vecsum(pos*pos) + vecsum(rayorig*rayorig) - 2.0f*vecsum(rayorig*pos) - rad*rad;
        float D = b*b + (-4.0f)*a*c;

        // If ray can not intersect then stop
        if (D < 0) {
            return false;
        }
        D = sqrtf(D);

        // Ray can intersect the sphere, solve the closer hitpoint
        float t = (-0.5f)*(b + D) / a;
        if (t > 0.0f) {
            distance = sqrtf(a)*t;
            hitpoint = rayorig + t*raydir;
            normal = (hitpoint - pos) / rad;
        } else {
            return false;
        }
        return true;
    }

    /*
    * Ray-box intersection using IEEE numerical properties to ensure that the
    * test is both robust and efficient, as described in:
    *
    *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
    *      "An Efficient and Robust Ray-Box Intersection Algorithm"
    *      Journal of graphics tools, 10(1):49-54, 2005
    *
    */
    /// @param corners: -x,-y,-z and +x,+y,+z corner positions
    /// @param dir: direction of ray
    /// @param start: origin of ray
    /// @param tmin: distance along ray for collision
    /// @return true on collision
    inline bool boxIntersect(const f32v3 corners[2], const f32v3& dir, const f32v3& start, OUT float& tmin) {
        float tmax, tymin, tymax, tzmin, tzmax;

        f32v3 invdir = 1.0f / dir;
        i32v3 sign;
        sign.x = (invdir.x) < 0;
        sign.y = (invdir.y) < 0;
        sign.z = (invdir.z) < 0;

        tmin = (corners[sign[0]].x - start.x) * invdir.x;
        tmax = (corners[1 - sign[0]].x - start.x) * invdir.x;
        tymin = (corners[sign[1]].y - start.y) * invdir.y;
        tymax = (corners[1 - sign[1]].y - start.y) * invdir.y;
        if ((tmin > tymax) || (tymin > tmax))
            return false;
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;
        tzmin = (corners[sign[2]].z - start.z) * invdir.z;
        tzmax = (corners[1 - sign[2]].z - start.z) * invdir.z;
        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;
        return true;
    }
}

#endif // IntersectionUtils_inl__
