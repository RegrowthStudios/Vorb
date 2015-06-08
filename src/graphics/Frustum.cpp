#include "stdafx.h"
#include "graphics/Frustum.h"

#include "Constants.h"

void vg::Frustum::Plane::setNormalAndPoint(const f32v3 &normal, const f32v3 &point) {
    this->normal = glm::normalize(normal);
    d = -(glm::dot(this->normal, point));
}

void vg::Frustum::Plane::setCoefficients(f32 a, f32 b, f32 c, f32 d) {
    //compute the length of the vector
    f32 l = glm::length(f32v3(a, b, c));
    // normalize the vector
    normal = f32v3(a / l, b / l, c / l);
    // and divide d by th length as well
    this->d = d / l;
}

f32 vg::Frustum::Plane::distance(const f32v3 &p) const {
    return (d + glm::dot(normal, p));
}

void vg::Frustum::setCamInternals(f32 fov, f32 aspectRatio, f32 znear, f32 zfar) {
#define RADIANS_PER_DEGREE 0.01745329251994329576923690768489
    // store the information
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_znear = znear;
    m_zfar = zfar;

    // compute width and height of the near and far plane sections
    f32 tang = (f32)tan(RADIANS_PER_DEGREE * (f64)fov * 0.5);
    m_nh = m_znear * tang;
    m_nw = m_nh * m_aspectRatio;
    m_fh = m_zfar  * tang;
    m_fw = m_fh * m_aspectRatio;
}

void vg::Frustum::updateFromWVP(const f32m4& WVP) {
    m_planes[P_NEAR].setCoefficients(
        WVP[0][2] + WVP[0][3],
        WVP[1][2] + WVP[1][3],
        WVP[2][2] + WVP[2][3],
        WVP[3][2] + WVP[3][3]);
    m_planes[P_FAR].setCoefficients(
        -WVP[0][2] + WVP[0][3],
        -WVP[1][2] + WVP[1][3],
        -WVP[2][2] + WVP[2][3],
        -WVP[3][2] + WVP[3][3]);
    m_planes[P_BOTTOM].setCoefficients(
        WVP[0][1] + WVP[0][3],
        WVP[1][1] + WVP[1][3],
        WVP[2][1] + WVP[2][3],
        WVP[3][1] + WVP[3][3]);
    m_planes[P_TOP].setCoefficients(
        -WVP[0][1] + WVP[0][3],
        -WVP[1][1] + WVP[1][3],
        -WVP[2][1] + WVP[2][3],
        -WVP[3][1] + WVP[3][3]);
    m_planes[P_LEFT].setCoefficients(
        WVP[0][0] + WVP[0][3],
        WVP[1][0] + WVP[1][3],
        WVP[2][0] + WVP[2][3],
        WVP[3][0] + WVP[3][3]);
    m_planes[P_RIGHT].setCoefficients(
        -WVP[0][0] + WVP[0][3],
        -WVP[1][0] + WVP[1][3],
        -WVP[2][0] + WVP[2][3],
        -WVP[3][0] + WVP[3][3]);
}

void vg::Frustum::update(const f32v3& position, const f32v3& dir, const f32v3& up) {

    f32v3 nc, fc, X, Y, Z;

    // Compute the Z axis of camera
    // This axis points in the opposite direction from 
    // the looking direction
    Z = glm::normalize(position - dir);

    // X axis of camera with given "up" vector and Z axis
    X = glm::normalize(glm::cross(up, Z));

    // The real "up" vector is the cross product of Z and X
    Y = glm::cross(Z, X);

    // compute the centers of the near and far planes
    nc = position - Z * m_znear;
    fc = position - Z * m_zfar;

    m_planes[P_NEAR].setNormalAndPoint(-Z, nc);
    m_planes[P_FAR].setNormalAndPoint(Z, fc);

    f32v3 aux, normal;

    aux = glm::normalize((nc + Y * m_nh) - position);
    normal = glm::cross(aux, X);
    m_planes[P_TOP].setNormalAndPoint(normal, nc + Y * m_nh);

    aux = glm::normalize((nc - Y * m_nh) - position);
    normal = glm::cross(X, aux);
    m_planes[P_BOTTOM].setNormalAndPoint(normal, nc - Y * m_nh);

    aux = glm::normalize((nc - X * m_nw) - position);
    normal = glm::cross(aux, Y);
    m_planes[P_LEFT].setNormalAndPoint(normal, nc - X * m_nw);

    aux = glm::normalize((nc + X * m_nw) - position);
    normal = glm::cross(Y, aux);
    m_planes[P_RIGHT].setNormalAndPoint(normal, nc + X * m_nw);
}

bool vg::Frustum::pointInFrustum(const f32v3& pos) const {
    for (int p = 0; p < 4; p++) { //*************************************** IGNORING FAR AND NEAR CLIPPING PLANE
        if (m_planes[p].distance(pos) <= 0) return false;
    }
    return true;
}

bool vg::Frustum::sphereInFrustum(const f32v3& pos, f32 radius) const {
    for (int p = 0; p < 4; p++) { //*************************************** IGNORING FAR AND NEAR CLIPPING PLANE
        if (m_planes[p].distance(pos) <= -radius) return false;
    }
    return true;
}
