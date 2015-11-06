#include "stdafx.h"
#include "graphics/Camera.h"

const f64v3 vg::Camera3D::ORIG_DIRECTION = f64v3(1.0, 0.0, 0.0);
const f64v3 vg::Camera3D::ORIG_RIGHT = f64v3(0.0, 0.0, 1.0);
const f64v3 vg::Camera3D::ORIG_UP = f64v3(0.0, 1.0, 0.0);

vg::Camera3D::Camera3D() {
    // Empty
}

void vg::Camera3D::init(f64 aspectRatio, f64 fieldOfView) {
    m_aspectRatio = aspectRatio;
    m_fieldOfView = fieldOfView;
}

void vg::Camera3D::update() {
    bool updateFrustum = false;
    if (m_viewChanged) {
        updateView();
        m_viewChanged = false;
        updateFrustum = true;
    }
    if (m_projectionChanged) {
        updateProjection();
        m_projectionChanged = false;
        updateFrustum = true;
    }

    if (updateFrustum) {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        m_frustum.updateFromWVP(m_viewProjectionMatrix);
    }
}

void vg::Camera3D::updateView() {
    m_viewMatrix = vmath::lookAt(f64v3(0.0f), getDirection(), getUp());
}

void vg::Camera3D::updateProjection() {
    m_frustum.setCamInternals(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
    m_projectionMatrix = vmath::perspective(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
}

void vg::Camera3D::offsetPosition(const f64v3& offset) {
    m_position += offset;
    m_viewChanged = true;
}

void vg::Camera3D::applyRotation(const f64q& rot) {
    m_directionQuat = rot * m_directionQuat;

    m_viewChanged = true;
}

void vg::Camera3D::applyRotation(f64 angle, const f64v3& axis) {
    applyRotation(vmath::angleAxis(angle, axis));
}

void vg::Camera3D::applyRoll(f64 angle) {
    applyRotation(vmath::angleAxis(angle, getDirection()));
}

void vg::Camera3D::applyYaw(f64 angle) {
    applyRotation(vmath::angleAxis(angle, getUp()));
}

void vg::Camera3D::applyPitch(f64 angle) {
    applyRotation(vmath::angleAxis(angle, getRight()));
}

void vg::Camera3D::rotateFromMouse(f64 dx, f64 dy, f64 speed) {
    f64q upQuat = vmath::angleAxis(dy * speed, getRight());
    f64q rightQuat = vmath::angleAxis(dx * speed, getUp());

    applyRotation(upQuat * rightQuat);
}

void vg::Camera3D::rollFromMouse(f64 dx, f64 speed) {
    f64q frontQuat = vmath::angleAxis(dx * speed, getDirection());

    applyRotation(frontQuat);
}

f64v3 vg::Camera3D::worldToScreenPoint(const f64v3& worldPoint) const {
    // Transform world to clipping coordinates
    f64v4 clipPoint = m_viewProjectionMatrix * f64v4(worldPoint, 1.0);
    clipPoint.x /= clipPoint.w;
    clipPoint.y /= clipPoint.w;
    clipPoint.z /= clipPoint.w;
    return f64v3((clipPoint.x + 1.0) / 2.0,
        (1.0 - clipPoint.y) / 2.0,
        clipPoint.z);
}

f64v3 vg::Camera3D::worldToScreenPointLogZ(const f64v3& worldPoint, f64 zFar) const {
    // Transform world to clipping coordinates
    f64v4 clipPoint = m_viewProjectionMatrix * f64v4(worldPoint, 1.0);
    clipPoint.z = log2(vmath::max(0.0001, clipPoint.w + 1.0)) * 2.0 / log2(zFar + 1.0) - 1.0;
    clipPoint.x /= clipPoint.w;
    clipPoint.y /= clipPoint.w;
    return f64v3((clipPoint.x + 1.0) / 2.0,
        (1.0 - clipPoint.y) / 2.0,
        clipPoint.z);
}

f64v3 vg::Camera3D::getPickRay(const f64v2& ndcScreenPos) const {
    f64v4 clipRay(ndcScreenPos.x, ndcScreenPos.y, -1.0, 1.0);
    f64v4 eyeRay = vmath::inverse(m_projectionMatrix) * clipRay;
    eyeRay = f64v4(eyeRay.x, eyeRay.y, -1.0, 0.0);
    return vmath::normalize(f64v3(vmath::inverse(m_viewMatrix) * eyeRay));
}
