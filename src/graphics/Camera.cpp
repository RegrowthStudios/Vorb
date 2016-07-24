#include "stdafx.h"
#include "graphics/Camera.h"

const f32v3 vg::Camera::UP_ABSOLUTE = f32v3(0.0f, 1.0f, 0.0f);

vg::Camera::Camera() {
    // Empty
}

vg::Camera::~Camera() {

}

void vg::Camera::init(f32 aspectRatio) {
    m_aspectRatio = aspectRatio;
}

void vg::Camera::update() {
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

void vg::Camera::offsetPosition(const f32v3& offset) {
    m_position += offset;
    m_viewChanged = true;
}

void vg::Camera::offsetPosition(const f64v3& offset) {
    m_position += offset;
    m_viewChanged = true;
}

void vg::Camera::rotate(const f32q& rot) {
    m_direction = rot * m_direction;
    m_left      = rot * m_left;
    m_up        = vmath::normalize(vmath::cross(m_left, m_direction));

    m_viewChanged = true;
}

void vg::Camera::rotate(f32 yaw, f32 pitch) {
    f32q upQuat    = vmath::angleAxis(pitch, m_left);
    f32q rightQuat = vmath::angleAxis(yaw, m_up);

    rotate(upQuat * rightQuat);
}

void vg::Camera::rotateAbsoluteUp(f32 yaw, f32 pitch, bool clampVerticalRotation /*= false*/) {
    f32q upQuat    = vmath::angleAxis(pitch, m_left);
    f32q rightQuat = vmath::angleAxis(yaw, UP_ABSOLUTE);

    f32v3 previousDirection = m_direction;
    f32v3 previousUp        = m_up;
    f32v3 previousRight     = m_left;

    rotate(upQuat * rightQuat);

    if (clampVerticalRotation && m_up.y < 0) {
        m_direction = previousDirection;
        m_up = previousUp;
        m_left = previousRight;
        rotateAbsoluteUp(yaw, 0.0f);
    }
}

void vg::Camera::roll(f32 roll) {
    f32q frontQuat = vmath::angleAxis(roll, m_direction);

    rotate(frontQuat);
}

void vg::Camera::setOrientation(const f32v3& direction, const f32v3& up) {
    m_direction = vmath::normalize(direction);
    m_up = vmath::normalize(up);
    m_left = vmath::cross(m_up, m_direction);
    // We calculate up again to guarantee orthogonality
    m_up = vmath::cross(m_direction, m_left);
}

void vg::Camera::setOrientation(const f64q& orientation) {
    m_direction = orientation * f64v3(0.0, 0.0, 1.0);
    m_left = orientation * f64v3(1.0, 0.0, 0.0);
    m_up = orientation * f64v3(0.0, 1.0, 0.0);
    m_viewChanged = true;
}

void vg::Camera::updateView() {
    // TODO(Ben): Allow camera relative view matrix.
    m_viewMatrix = vmath::lookAt(f32v3(m_position), f32v3(m_position) + m_direction, -m_up);
}

void vg::Camera::updateProjection() {
    m_frustum.setCamInternals(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
    m_projectionMatrix = vmath::perspective(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
}
