//
// Camera.h
// Vorb Engine
//
// Created by Matthew Marshall on 4 Nov 2015
// Copyright 2015 Regrowth Studios
// All Rights Reserved
//

/*! \file Camera.h
* @brief Defines various camera types.
*/

#pragma once

#ifndef Vorb_Camera_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Camera_h__
//! @endcond 

#include "Frustum.h"
#include "../math/TweeningMath.hpp"
#include "../types.h"

namespace vorb {
    namespace graphics {
        /*! @brief Basic camera class, offering standard camera functionality.
        */
        template <typename T>
        class Camera3D {
        protected:
            typedef T fXX;
            typedef Vector2<T> fXXv2;
            typedef Vector3<T> fXXv3;
            typedef Vector4<T> fXXv4;
            typedef Quaternion<T> fXXq;
            typedef Matrix4<T> fXXm4;
        public:
            Camera3D();

            void init(fXX aspectRatio, fXX fieldOfView);
            virtual void update(fXX deltaTime);

            virtual void applyRotation(const fXXq& rot);
            virtual void applyRotation(fXX angle, const fXXv3& axis);
            virtual void applyRoll(fXX angle);
            virtual void applyYaw(fXX angle);
            virtual void applyPitch(fXX angle);
            virtual void rotateFromMouse(fXX dx, fXX dy, fXX speed);
            virtual void rollFromMouse(fXX dx, fXX speed);

            void offsetPosition(const fXXv3& offset);

            // Setters
            void setOrientation(const fXXq& orientation) { m_directionQuat = orientation; m_viewChanged = 1; }
            void setFocalPoint(const fXXv3& focalPoint) { m_focalPoint = focalPoint; m_viewChanged = 1; }
            void setPosition(const fXXv3& position) { m_focalPoint = position; m_position = position; m_focalLength = 0;  m_viewChanged = 1; }
            void setClippingPlane(fXX zNear, fXX zFar){ m_zNear = zNear; m_zFar = zFar; m_projectionChanged = 1; }
            void setFieldOfView(fXX fieldOfView){ m_fieldOfView = fieldOfView; m_projectionChanged = 1; }
            void setFocalLength(fXX focalLength) { m_focalLength = focalLength; m_viewChanged = 1; }
            void setAspectRatio(fXX aspectRatio) { m_aspectRatio = aspectRatio; m_projectionChanged = 1; }

            // Getters
            const fXXv3& getPosition() const { return m_position; }
            const fXXv3& getDirection() const { return vmath::normalize(m_directionQuat * ORIG_DIRECTION); }
            const fXXv3& getRight() const { return vmath::normalize(m_directionQuat * ORIG_RIGHT); }
            const fXXv3& getUp() const { return vmath::normalize(m_directionQuat * ORIG_UP); }
            const fXX&   getRoll() const { return vmath::roll(m_directionQuat); }
            const fXX&   getPitch() const { return vmath::pitch(m_directionQuat); }
            const fXX&   getYaw() const { return vmath::yaw(m_directionQuat); }

            const fXXm4& getProjectionMatrix() const { return m_projectionMatrix; }
            const fXXm4& getViewMatrix() const { return m_viewMatrix; }
            const fXXm4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

            const fXX& getNearClip() const { return m_zNear; }
            const fXX& getFarClip() const { return m_zFar; }
            const fXX& getFieldOfView() const { return m_fieldOfView; }
            const fXX& getAspectRatio() const { return m_aspectRatio; }
            const fXX& getFocalLength() const { return m_focalLength; }

            // Gets the position of a 3D point on the screen plane
            fXXv3 worldToScreenPoint(const fXXv3& worldPoint) const;
            fXXv3 worldToScreenPointLogZ(const fXXv3& worldPoint, fXX zFar) const;
            fXXv3 getPickRay(const fXXv2& ndcScreenPos) const;

            // Frustum wrappers
            bool pointInFrustum(const fXXv3& pos) const { return m_frustum.pointInFrustum(pos); }
            bool sphereInFrustum(const fXXv3& pos, fXX radius) const { return m_frustum.sphereInFrustum(pos, radius); }

            const Frustum& getFrustum() const { return m_frustum; }
        protected:
            void updateView();
            void updateProjection();

            fXX m_zNear = 0.1;
            fXX m_zFar = 100000.0;
            fXX m_fieldOfView = 75.0;
            fXX m_aspectRatio = 4.0 / 3.0;
            fXX m_focalLength = 0.0;
            fXX m_maxFocalLength = 10000000000000000000000.0;
            bool m_viewChanged = true;
            bool m_projectionChanged = true;

            fXXv3 m_focalPoint = fXXv3(0.0);
            fXXv3 m_position = fXXv3(0.0);
            fXXq m_directionQuat;

            static const fXXv3 ORIG_DIRECTION;
            static const fXXv3 ORIG_RIGHT;
            static const fXXv3 ORIG_UP;

            fXXm4 m_projectionMatrix;
            fXXm4 m_viewMatrix;
            fXXm4 m_viewProjectionMatrix;

            Frustum m_frustum; ///< For frustum culling
        };

        /*! @brief Cinematic camera class, offering various tools for cinematic camera shots.
        */
        template <typename T>
        class CinematicCamera3D : public Camera3D<T> {
        protected:
            struct CameraPathControlPoint {
                fXXv3 position;
                fXXv3 orientation;
                fXX focalLength;
                fXX fieldOfView;
                CameraPathControlPoint()
                    : position(fXXv3(0.0)),
                    orientation(fXXv3(0.0)),
                    focalLength((fXX)0.0),
                    fieldOfView((fXX)0.0) {
                }
                CameraPathControlPoint(fXXv3 pos, fXXv3 dir, fXX focLen, fXX fov)
                    : position(pos),
                    orientation(dir),
                    focalLength(focLen),
                    fieldOfView(fov) {
                }
            };
            struct CameraPathFixedPoint : CameraPathControlPoint {
                fXX period;
                fXX(*positionalTweeningFunc)(fXX, fXX, fXX);
                fXX(*orientationTweeningFunc)(fXX, fXX, fXX);
                fXX(*focalLengthTweeningFunc)(fXX, fXX, fXX);
                fXX(*fieldOfViewTweeningFunc)(fXX, fXX, fXX);
                CameraPathFixedPoint()
                    : CameraPathControlPoint(),
                    period((fXX)0.0),
                    positionalTweeningFunc(nullptr),
                    orientationTweeningFunc(nullptr), 
                    focalLengthTweeningFunc(nullptr),
                    fieldOfViewTweeningFunc(nullptr) {
                }
                CameraPathFixedPoint(fXXv3 pos, fXXv3 dir, fXX focLen, fXX fov,
                    fXX dur, fXX(*posTweeningFunc)(fXX, fXX, fXX),
                    fXX(*dirTweeningFunc)(fXX, fXX, fXX),
                    fXX(*focLenTweeningFunc)(fXX, fXX, fXX),
                    fXX(*fovTweeningFunc)(fXX, fXX, fXX))
                    : CameraPathControlPoint(pos, dir, focLen, fov),
                    period(dur),
                    positionalTweeningFunc(posTweeningFunc),
                    orientationTweeningFunc(dirTweeningFunc),
                    focalLengthTweeningFunc(focLenTweeningFunc),
                    fieldOfViewTweeningFunc(fovTweeningFunc) {
                }
            };
        public:
            CinematicCamera3D();

            virtual void update(fXX deltaTime) override;

            void init();
            void addActualPointToPath(fXXv3 position, fXXv3 orientation, fXX period, fXX(*positionalTweeningFunc)(fXX, fXX, fXX) = &vmath::linear, fXX(*orientationTweeningFunc)(fXX, fXX, fXX) = &vmath::linear, fXX focalLength = (fXX)0.0, fXX(*focalLengthTweeningFunc)(fXX, fXX, fXX) = &vmath::linear, fXX fieldOfView = (fXX)-1.0, fXX(*fieldOfViewTweeningFunc)(fXX, fXX, fXX) = &vmath::linear);
            void addControlPointToPath(fXXv3 position, fXXv3 orientation, fXX focalLength = (fXX)0.0, fXX fieldOfView = (fXX)-1.0);
            void begin();
        private:
            void clearPath();
            void updatePath(fXX deltaTime);

            std::deque<std::pair<CameraPathFixedPoint, std::vector<CameraPathControlPoint>>> m_path;
            bool m_running = false;
            fXX m_timeElapsed = (fXX)0.0;
        };

        /*! @brief FPS camera class, offering various tools for maintaining a FPS-style camera.
        */
        template <typename T>
        class FPSCamera3D : public Camera3D<T> {
        public:
            FPSCamera3D();

            virtual void update(fXX deltaTime) override;

            virtual void applyRotation(const fXXq& rot) override;

            void stabiliseRoll();

            // Setters
            void setWobbleAmplitude(fXX amplitude) { m_wobbleAmplitude = amplitude; }
            void enableWobble(bool enable) {
                m_wobbleEnabled = enable;
                if (enable) {
                    m_lockRoll = false;
                }
            }
            void setWobbleTweening(fXX(*tweeningFunc)(fXX, fXX, fXX)) { m_wobbleTween = tweeningFunc; }
            void resetWobble() { m_wobbleStage = (T)0.0; }
            void setPitchLimit(fXX magnitude) { m_pitchLimit = magnitude; }
            void lockPitch(bool lock) { m_lockPitch = lock; }
            void setRollLimit(fXX magnitude) { m_pitchLimit = magnitude; }
            void lockRoll(bool lock) {
                m_lockRoll = lock;
                if (lock) {
                    m_wobbleEnabled = false;
                }
            }
        private:
            void updateWobble(fXX deltaTime);

            fXX m_wobbleAmplitude = (fXX)0.0;
            fXX m_wobblePeriod = (fXX)0.0;
            fXX m_wobbleStage = (fXX)0.0; ///< Goes between -m_wobblePeriod and m_wobblePeriod
            fXX(*m_wobbleTween)(fXX, fXX, fXX) = &vmath::easeInOutSine;
            bool m_wobbleEnabled = false;

            fXX m_pitchLimit = (fXX)M_PIF;
            bool m_lockPitch = true;
            fXX m_rollLimit = (fXX)0.0;
            bool m_lockRoll = true;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_Camera_h__
