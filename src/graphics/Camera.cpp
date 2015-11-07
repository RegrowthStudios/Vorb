#include "stdafx.h"
#include "graphics/Camera.h"

//////////////////////////////////////////////////////////////////////////
//                          General 3D Camera                           //
//////////////////////////////////////////////////////////////////////////

template <class T>
const fXXv3 vg::Camera3D<T>::ORIG_DIRECTION = fXXv3(1.0, 0.0, 0.0);
template <class T>
const fXXv3 vg::Camera3D<T>::ORIG_RIGHT = fXXv3(0.0, 0.0, 1.0);
template <class T>
const fXXv3 vg::Camera3D<T>::ORIG_UP = fXXv3(0.0, 1.0, 0.0);

template <class T>
vg::Camera3D<T>::Camera3D() {
    static_assert(std::numeric_limits<T>::is_iec559, "Camera3D only accepts floating-point template types.");
}

template <class T>
void vg::Camera3D<T>::init(fXX aspectRatio, fXX fieldOfView) {
    m_aspectRatio = aspectRatio;
    m_fieldOfView = fieldOfView;
}

template <class T>
void vg::Camera3D<T>::update() {
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

template <class T>
void vg::Camera3D<T>::updateView() {
    m_viewMatrix = vmath::lookAt(fXXv3(0.0f), getDirection(), getUp());
}

template <class T>
void vg::Camera3D<T>::updateProjection() {
    m_frustum.setCamInternals(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
    m_projectionMatrix = vmath::perspective(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
}

template <class T>
void vg::Camera3D<T>::offsetPosition(const fXXv3& offset) {
    m_position += offset;
    m_viewChanged = true;
}

template <class T>
void vg::Camera3D<T>::applyRotation(const fXXq& rot) {
    m_directionQuat = rot * m_directionQuat;

    m_viewChanged = true;
}

template <class T>
void vg::Camera3D<T>::applyRotation(fXX angle, const fXXv3& axis) {
    applyRotation(vmath::angleAxis(angle, axis));
}

template <class T>
void vg::Camera3D<T>::applyRoll(fXX angle) {
    applyRotation(vmath::angleAxis(angle, getDirection()));
}

template <class T>
void vg::Camera3D<T>::applyYaw(fXX angle) {
    applyRotation(vmath::angleAxis(angle, getUp()));
}

template <class T>
void vg::Camera3D<T>::applyPitch(fXX angle) {
    applyRotation(vmath::angleAxis(angle, getRight()));
}

template <class T>
void vg::Camera3D<T>::rotateFromMouse(fXX dx, fXX dy, fXX speed) {
    fXXq upQuat = vmath::angleAxis(dy * speed, getRight());
    fXXq rightQuat = vmath::angleAxis(dx * speed, getUp());

    applyRotation(upQuat * rightQuat);
}

template <class T>
void vg::Camera3D<T>::rollFromMouse(fXX dx, fXX speed) {
    fXXq frontQuat = vmath::angleAxis(dx * speed, getDirection());

    applyRotation(frontQuat);
}

template <class T>
fXXv3 vg::Camera3D<T>::worldToScreenPoint(const fXXv3& worldPoint) const {
    // Transform world to clipping coordinates
    fXXv4 clipPoint = m_viewProjectionMatrix * fXXv4(worldPoint, 1.0);
    clipPoint.x /= clipPoint.w;
    clipPoint.y /= clipPoint.w;
    clipPoint.z /= clipPoint.w;
    return fXXv3((clipPoint.x + 1.0) / 2.0,
        (1.0 - clipPoint.y) / 2.0,
        clipPoint.z);
}

template <class T>
fXXv3 vg::Camera3D<T>::worldToScreenPointLogZ(const fXXv3& worldPoint, fXX zFar) const {
    // Transform world to clipping coordinates
    fXXv4 clipPoint = m_viewProjectionMatrix * fXXv4(worldPoint, 1.0);
    clipPoint.z = log2(vmath::max(0.0001, clipPoint.w + 1.0)) * 2.0 / log2(zFar + 1.0) - 1.0;
    clipPoint.x /= clipPoint.w;
    clipPoint.y /= clipPoint.w;
    return fXXv3((clipPoint.x + 1.0) / 2.0,
        (1.0 - clipPoint.y) / 2.0,
        clipPoint.z);
}

template <class T>
fXXv3 vg::Camera3D<T>::getPickRay(const fXXv2& ndcScreenPos) const {
    fXXv4 clipRay(ndcScreenPos.x, ndcScreenPos.y, -1.0, 1.0);
    fXXv4 eyeRay = vmath::inverse(m_projectionMatrix) * clipRay;
    eyeRay = fXXv4(eyeRay.x, eyeRay.y, -1.0, 0.0);
    return vmath::normalize(fXXv3(vmath::inverse(m_viewMatrix) * eyeRay));
}

//////////////////////////////////////////////////////////////////////////
//                         Cinematic 3D Camera                          //
//////////////////////////////////////////////////////////////////////////

// Nothing so far.

//////////////////////////////////////////////////////////////////////////
//                            FPS 3D Camera                             //
//////////////////////////////////////////////////////////////////////////

template <class T>
void vg::FPSCamera3D<T>::applyRotation(const fXXq& rot) {
    m_directionQuat = rot * m_directionQuat;

    if (m_lockPitch) {
        T newPitch = vmath::pitch(m_directionQuat);
        if (newPitch > m_pitchLimit) {
            T deltaPitch = newPitch - m_pitchLimit;
            applyPitch(deltaPitch);
        } else if (newPitch < (T)-1.0 * m_pitchLimit) {
            T deltaPitch = newPitch + m_pitchLimit;
            applyPitch(deltaPitch);
        }
    }

    if (m_lockRoll) {
        T newRoll = vmath::roll(m_directionQuat);
        if (newroll > m_rollLimit) {
            T deltaRoll = newRoll - m_rollLimit;
            applyRoll(deltaRoll);
        } else if (newRoll < (T)-1.0 * m_rollLimit) {
            T deltaRoll = newRoll + m_rollLimit;
            applyRoll(deltaRoll);
        }
    }

    m_viewChanged = true;
}

template <class T>
void vg::FPSCamera3D<T>::stabiliseRoll() {
    T currRoll = vmath::roll(m_directionQuat);
    applyRoll((T)-1.0 * currRoll);
}