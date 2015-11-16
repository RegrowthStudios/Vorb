#include "stdafx.h"
#include "graphics/Camera.h"

#include "math/BezierMath.hpp"

//////////////////////////////////////////////////////////////////////////
//                          General 3D Camera                           //
//////////////////////////////////////////////////////////////////////////

#define FXX typename vg::Camera3D<T>::fXX
#define FXXV2 typename vg::Camera3D<T>::fXXv2
#define FXXV3 typename vg::Camera3D<T>::fXXv3
#define FXXV4 typename vg::Camera3D<T>::fXXv4
#define FXXQ typename vg::Camera3D<T>::fXXq
#define FXXM4 typename vg::Camera3D<T>::fXXm4

template <typename T>
const FXXV3 vg::Camera3D<T>::ORIG_DIRECTION = FXXV3(1.0, 0.0, 0.0);
template <typename T>
const FXXV3 vg::Camera3D<T>::ORIG_RIGHT = FXXV3(0.0, 0.0, 1.0);
template <typename T>
const FXXV3 vg::Camera3D<T>::ORIG_UP = FXXV3(0.0, 1.0, 0.0);

template <typename T>
vg::Camera3D<T>::Camera3D() {
    static_assert(std::numeric_limits<T>::is_iec559, "Camera3D only accepts floating-point class types.");
}

template <typename T>
void vg::Camera3D<T>::init(FXX aspectRatio, FXX fieldOfView) {
    m_aspectRatio = aspectRatio;
    m_fieldOfView = fieldOfView;
}

template <typename T>
void vg::Camera3D<T>::update(FXX deltaTime) {
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

template <typename T>
void vg::Camera3D<T>::updateView() {
    m_viewMatrix = vmath::lookAt(FXXV3(0.0), getDirection(), getUp());
}

template <typename T>
void vg::Camera3D<T>::updateProjection() {
    m_frustum.setCamInternals(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
    m_projectionMatrix = vmath::perspective(m_fieldOfView, m_aspectRatio, m_zNear, m_zFar);
}

template <typename T>
void vg::Camera3D<T>::offsetPosition(const FXXV3& offset) {
    m_position += offset;
    m_viewChanged = true;
}

template <typename T>
void vg::Camera3D<T>::applyRotation(const FXXQ& rot) {
    m_directionQuat = vmath::normalize(m_directionQuat * vmath::normalize(rot));

    m_viewChanged = true;
}

template <typename T>
void vg::Camera3D<T>::applyRotation(FXX angle, const FXXV3& axis) {
    applyRotation(vmath::angleAxis(angle, axis));
}

template <typename T>
void vg::Camera3D<T>::applyRoll(FXX angle) {
    applyRotation(vmath::angleAxis(angle, getDirection()));
}

template <typename T>
void vg::Camera3D<T>::applyYaw(FXX angle) {
    applyRotation(vmath::angleAxis(angle, getUp()));
}

template <typename T>
void vg::Camera3D<T>::applyPitch(FXX angle) {
    applyRotation(vmath::angleAxis(angle, getRight()));
}

template <typename T>
void vg::Camera3D<T>::rotateFromMouse(FXX dx, FXX dy, FXX speed) {
    FXXQ upQuat = vmath::angleAxis(dy * speed, getRight());
    FXXQ rightQuat = vmath::angleAxis(dx * speed, getUp());

    applyRotation(upQuat * rightQuat);
}

template <typename T>
void vg::Camera3D<T>::rollFromMouse(FXX dx, FXX speed) {
    FXXQ frontQuat = vmath::angleAxis(dx * speed, getDirection());

    applyRotation(frontQuat);
}

template <typename T>
FXXV3 vg::Camera3D<T>::worldToScreenPoint(const FXXV3& worldPoint) const {
    // Transform world to clipping coordinates
    FXXV4 clipPoint = m_viewProjectionMatrix * FXXV4(worldPoint, 1.0);
    clipPoint.x /= clipPoint.w;
    clipPoint.y /= clipPoint.w;
    clipPoint.z /= clipPoint.w;
    return FXXV3((clipPoint.x + (FXX)1.0) / (FXX)2.0,
        ((FXX)1.0 - clipPoint.y) / (FXX)2.0,
        clipPoint.z);
}

template <typename T>
FXXV3 vg::Camera3D<T>::worldToScreenPointLogZ(const FXXV3& worldPoint, FXX zFar) const {
    // Transform world to clipping coordinates
    FXXV4 clipPoint = m_viewProjectionMatrix * FXXV4(worldPoint, 1.0);
    clipPoint.z = log2(vmath::max((FXX)0.0001, clipPoint.w + (FXX)1.0)) * (FXX)2.0 / log2(zFar + (FXX)1.0) - (FXX)1.0;
    clipPoint.x /= clipPoint.w;
    clipPoint.y /= clipPoint.w;
    return FXXV3((clipPoint.x + (FXX)1.0) / (FXX)2.0,
        ((FXX)1.0 - clipPoint.y) / (FXX)2.0,
        clipPoint.z);
}

template <typename T>
FXXV3 vg::Camera3D<T>::getPickRay(const FXXV2& ndcScreenPos) const {
    FXXV4 clipRay(ndcScreenPos.x, ndcScreenPos.y, -1.0, 1.0);
    FXXV4 eyeRay = vmath::inverse(m_projectionMatrix) * clipRay;
    eyeRay = FXXV4(eyeRay.x, eyeRay.y, -1.0, 0.0);
    return vmath::normalize(FXXV3(vmath::inverse(m_viewMatrix) * eyeRay));
}

//////////////////////////////////////////////////////////////////////////
//                         Cinematic 3D Camera                          //
//////////////////////////////////////////////////////////////////////////

template <typename T>
vg::CinematicCamera3D<T>::CinematicCamera3D() {
    static_assert(std::numeric_limits<T>::is_iec559, "CinematicCamera3D only accepts floating-point class types.");
}

template <typename T>
void vg::CinematicCamera3D<T>::update(FXX deltaTime) {
    updatePath(deltaTime);

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

template <typename T>
void vg::CinematicCamera3D<T>::init() {
    clearPath();
    m_running = false;
}

template <typename T>
void vg::CinematicCamera3D<T>::addActualPointToPath(FXXV3 position, FXXV3 orientation, FXX period, FXX(*positionalTweeningFunc)(FXX, FXX, FXX) /* = &vmath::linear */, FXX(*orientationTweeningFunc)(FXX, FXX, FXX) /* = &vmath::linear */, FXX focalLength /* = (fXX)0.0 */, FXX(*focalLengthTweeningFunc)(FXX, FXX, FXX) /* = &vmath::linear */, FXX fieldOfView /* = (fXX)-1.0 */, FXX(*fieldOfViewTweeningFunc)(FXX, FXX, FXX) /* = &vmath::linear */) {
    CameraPathFixedPoint actualPoint = CameraPathFixedPoint(
        position,
        orientation,
        focalLength,
        fieldOfView,
        period,
        positionalTweeningFunc,
        orientationTweeningFunc,
        focalLengthTweeningFunc,
        fieldOfViewTweeningFunc
    );
    m_path.emplace_back(actualPoint, std::vector<CameraPathControlPoint>());
}

template <typename T>
void vg::CinematicCamera3D<T>::addControlPointToPath(FXXV3 position, FXXV3 orientation, FXX focalLength /* = (fXX)0.0 */, FXX fieldOfView /* = (fXX)-1.0 */) {
    CameraPathControlPoint controlPoint = CameraPathControlPoint(
        position,
        orientation,
        focalLength,
        fieldOfView
    );
    m_path.back().second.push_back(controlPoint);
}

template <typename T>
void vg::CinematicCamera3D<T>::begin() {
    CameraPathFixedPoint startPoint = m_path.front().first;
    setPosition(startPoint.position);
    setOrientation(FXXQ(startPoint.orientation));
    setFocalLength(startPoint.focalLength);
    setFieldOfView(startPoint.fieldOfView);
    m_running = true;
}

template <typename T>
void vg::CinematicCamera3D<T>::clearPath() {
    while (m_path.size() > 0) {
        m_path.pop_front();
    }
}

template <typename T>
void vg::CinematicCamera3D<T>::updatePath(FXX deltaTime) {
    CameraPathFixedPoint beginPoint = m_path[0].first;
    CameraPathFixedPoint endPoint = m_path[1].first;
    std::vector<CameraPathControlPoint> controlPoints = m_path[0].second;

    std::vector<FXXV3> controlPositions;
    std::vector<FXXV3> controlOrientations;
    std::vector<FXX> controlFocalLengths;
    std::vector<FXX> controlFieldOfViews;
    controlPositions.push_back(beginPoint.position);
    controlOrientations.push_back(beginPoint.orientation);
    controlFocalLengths.push_back(beginPoint.focalLength);
    controlFieldOfViews.push_back(beginPoint.fieldOfView);
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        controlPositions.push_back(controlPoints[i].position);
        controlOrientations.push_back(controlPoints[i].orientation);
        controlFocalLengths.push_back(controlPoints[i].focalLength);
        controlFieldOfViews.push_back(controlPoints[i].fieldOfView);
    }
    controlPositions.push_back(endPoint.position);
    controlOrientations.push_back(endPoint.orientation);
    controlFocalLengths.push_back(endPoint.focalLength);
    controlFieldOfViews.push_back(endPoint.fieldOfView);

    // TODO(Matthew): Assess if a weighting is needed when transitioning from a high dx/dt to a low dx/dt in any of the categories.
    // Clarification:
    //     If two path segments (path between two actual points) have same period and one is shorter in delta, or the same delta with
    //     one having a shorter period, it may be necessary to add some weighting factor so that the transition between the two is nice.

    // Calculate position change.
    FXX effectivePosAlpha = beginPoint.positionalTweeningFunc(0.0, 1.0, m_timeElapsed / beginPoint.period);
    FXXV3 newLocation = vmath::bezier3d<FXX>(effectivePosAlpha, controlPositions);
    setPosition(newLocation);

    // Calculate orientation change.
    FXX effectiveOriAlpha = beginPoint.orientationTweeningFunc(0.0, 1.0, m_timeElapsed / beginPoint.period);
    FXXV3 newOrientation = vmath::bezier3d<FXX>(effectiveOriAlpha, controlOrientations);
    setOrientation(Quaternion<FXX>(newOrientation));

    // Calculate focal length change.
    FXX effectiveFocLenAlpha = beginPoint.focalLengthTweeningFunc(0.0, 1.0, m_timeElapsed / beginPoint.period);
    FXX newFocalLength = vmath::bezier1d<FXX>(effectiveFocLenAlpha, controlFocalLengths);
    setFocalLength(newFocalLength);

    // Calculate field of view change.
    FXX effectiveFovAlpha = beginPoint.fieldOfViewTweeningFunc(0.0, 1.0, m_timeElapsed / beginPoint.period);
    FXX newFov = vmath::bezier1d<FXX>(effectiveFovAlpha, controlFieldOfViews);
    setFieldOfView(newFov);
     
    m_timeElapsed += deltaTime;
    if (m_timeElapsed > beginPoint.period) {
        m_timeElapsed -= beginPoint.period;
        m_path.pop_front();
        updatePath((FXX)0.0);
    }
}

//////////////////////////////////////////////////////////////////////////
//                            FPS 3D Camera                             //
//////////////////////////////////////////////////////////////////////////

template <typename T>
vg::FPSCamera3D<T>::FPSCamera3D() {
    static_assert(std::numeric_limits<T>::is_iec559, "FPSCamera3D only accepts floating-point class types.");
}

template <typename T>
void vg::FPSCamera3D<T>::update(FXX deltaTime) {
    if (m_wobbleEnabled) {
        updateWobble(deltaTime);
    }

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

template <typename T>
void vg::FPSCamera3D<T>::updateWobble(FXX deltaTime) {
    m_wobbleStage += deltaTime;
    if (m_wobbleStage > m_wobblePeriod) {
        FXX deltaStage = m_wobbleStage - m_wobblePeriod;
        m_wobbleStage = ((T)-1.0 * m_wobblePeriod) + deltaStage;
    }

    FXX targetRoll = m_wobbleTween((T)-1.0 * m_wobbleAmplitude, m_wobbleAmplitude, vmath::abs(m_wobbleStage) / m_wobblePeriod);
    FXX currentRoll = vmath::roll(m_directionQuat);
    FXX deltaRoll = targetRoll - currentRoll;

    m_directionQuat = vmath::angleAxis(deltaRoll, getDirection()) * m_directionQuat;
}

template <typename T>
void vg::FPSCamera3D<T>::applyRotation(const FXXQ& rot) {
    m_directionQuat = vmath::normalize(m_directionQuat * vmath::normalize(rot));

    if (0 && m_lockPitch) {
        T newPitch = vmath::pitch(m_directionQuat);
        if (newPitch - m_pitchLimit > (FXX)0.00001) {
            T deltaPitch = newPitch - m_pitchLimit;
            m_directionQuat = vmath::normalize(m_directionQuat * vmath::normalize(vmath::angleAxis(deltaPitch, getRight())));
        } else if (newPitch + m_pitchLimit > (FXX)0.00001) {
            T deltaPitch = newPitch + m_pitchLimit;
            m_directionQuat = vmath::normalize(m_directionQuat * vmath::normalize(vmath::angleAxis(deltaPitch, getRight())));
        }
    }

    if (0 && m_lockRoll) {
        T newRoll = vmath::roll(m_directionQuat);
        if (newRoll - m_rollLimit > (FXX)0.00001) {
            T deltaRoll = newRoll - m_rollLimit;
            m_directionQuat = vmath::normalize(m_directionQuat * vmath::normalize(vmath::angleAxis(deltaRoll, getDirection())));
        } else if (newRoll + m_rollLimit < (FXX)0.00001) {
            T deltaRoll = newRoll + m_rollLimit;
            m_directionQuat = vmath::normalize(m_directionQuat * vmath::normalize(vmath::angleAxis(deltaRoll, getDirection())));
        }
    }

    m_viewChanged = true;
}

template <typename T>
void vg::FPSCamera3D<T>::stabiliseRoll() {
    T currRoll = vmath::roll(m_directionQuat);
    applyRoll((FXX)-1.0 * currRoll);
}

namespace vorb {
    namespace graphics {
        template class Camera3D<f32>;
        template class Camera3D<f64>;
        template class CinematicCamera3D<f32>;
        template class CinematicCamera3D<f64>;
        template class FPSCamera3D<f32>;
        template class FPSCamera3D<f64>;
    }
}