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

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Frustum.h"

namespace vorb {
    namespace graphics {
        /*! @brief Basic camera class, offering standard camera functionality.
        */
        //template <class T>
        class Camera3D {
            /*template <typename T> struct vec_2_s;
            template <> struct vec_2_s<f32> {
                typedef f32v2 type;
            };
            template <> struct vec_2_s<f64> {
                typedef f64v2 type;
            };
            template <typename T> struct vec_3_s;
            template <> struct vec_3_s<f32> {
                typedef f32v3 type;
            };
            template <> struct vec_3_s<f64> {
                typedef f64v3 type;
            };
            template <typename T> struct quat_s;
            template <> struct quat_s<f32> {
                typedef f32q type;
            };
            template <> struct quat_s<f64> {
                typedef f64q type;
            };
            template <typename T> struct mat_4_s;
            template <> struct mat_4_s<f32> {
                typedef f32m4 type;
            };
            template <> struct mat_4_s<f64> {
                typedef f64m4 type;
            };*/
        public:

            Camera3D();
            void init(f64 aspectRatio, f64 fieldOfView);
            void update();

            void offsetPosition(const f64v3& offset);

            virtual void applyRotation(const f64q& rot);
            virtual void applyRotation(f64 angle, const f64v3& axis);
            virtual void applyRoll(f64 angle);
            virtual void applyYaw(f64 angle);
            virtual void applyPitch(f64 angle);
            virtual void rotateFromMouse(f64 dx, f64 dy, f64 speed);
            virtual void rollFromMouse(f64 dx, f64 speed);

            // Setters
            void setOrientation(const f64q& orientation);
            void setFocalPoint(const f64v3& focalPoint) { m_focalPoint = focalPoint; m_viewChanged = 1; }
            void setPosition(const f64v3& position) { m_focalPoint = position; m_position = position; m_focalLength = 0;  m_viewChanged = 1; }
            void setClippingPlane(f64 zNear, f64 zFar){ m_zNear = zNear; m_zFar = zFar; m_projectionChanged = 1; }
            void setFieldOfView(f64 fieldOfView){ m_fieldOfView = fieldOfView; m_projectionChanged = 1; }
            void setFocalLength(f64 focalLength) { m_focalLength = focalLength; m_viewChanged = 1; }
            void setAspectRatio(f64 aspectRatio) { m_aspectRatio = aspectRatio; m_projectionChanged = 1; }

            // Getters
            const f64v3& getPosition() const { return m_position; }
            const f64v3& getDirection() const { return m_directionQuat * ORIG_DIRECTION; }
            const f64v3& getRight() const { return m_directionQuat * ORIG_RIGHT; }
            const f64v3& getUp() const { return m_directionQuat * ORIG_UP; }

            const f64m4& getProjectionMatrix() const { return m_projectionMatrix; }
            const f64m4& getViewMatrix() const { return m_viewMatrix; }
            const f64m4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

            const f64& getNearClip() const { return m_zNear; }
            const f64& getFarClip() const { return m_zFar; }
            const f64& getFieldOfView() const { return m_fieldOfView; }
            const f64& getAspectRatio() const { return m_aspectRatio; }
            const f64& getFocalLength() const { return m_focalLength; }

            // Gets the position of a 3D point on the screen plane
            f64v3 worldToScreenPoint(const f64v3& worldPoint) const;
            f64v3 worldToScreenPointLogZ(const f64v3& worldPoint, f64 zFar) const;
            f64v3 getPickRay(const f64v2& ndcScreenPos) const;

            // Frustum wrappers
            bool pointInFrustum(const f64v3& pos) const { return m_frustum.pointInFrustum(pos); }
            bool sphereInFrustum(const f64v3& pos, f64 radius) const { return m_frustum.sphereInFrustum(pos, radius); }
        protected:
            void updateView();
            void updateProjection();

            f64 m_zNear = 0.1;
            f64 m_zFar = 100000.0;
            f64 m_fieldOfView = 75.0;
            f64 m_aspectRatio = 4.0 / 3.0;
            f64 m_focalLength = 0.0;
            f64 m_maxFocalLength = 10000000000000000000000.0;
            bool m_viewChanged = true;
            bool m_projectionChanged = true;

            f64v3 m_focalPoint = f64v3(0.0);
            f64v3 m_position = f64v3(0.0);
            f64q m_directionQuat;

            static const f64v3 ORIG_DIRECTION;
            static const f64v3 ORIG_RIGHT;
            static const f64v3 ORIG_UP;

            f64m4 m_projectionMatrix;
            f64m4 m_viewMatrix;
            f64m4 m_viewProjectionMatrix;

            Frustum m_frustum; ///< For frustum culling
        };

        /*! @brief Cinematic camera class, offering various tools for cinematic camera shots.
        */
        class CinematicCamera3D : public Camera3D {
        public:
        private:
        };

        /*! @brief FPS camera class, offering various tools for maintaining a FPS-style camera.
        */
        class FPSCamera3D : public Camera3D {
        public:
            virtual void applyRotation(const f64q& rot) override;
            virtual void applyRotation(f64 angle, const f64v3& axis) override;
            virtual void applyRoll(f64 angle) override;
            virtual void applyYaw(f64 angle) override;
            virtual void applyPitch(f64 angle) override;
            virtual void rotateFromMouse(f64 dx, f64 dy, f64 speed) override;
            virtual void rollFromMouse(f64 dx, f64 speed) override;

            // Setters
            void setWobbleMagnitude(f64 magnitude) { m_wobbleMagnitude = magnitude; }
            void enableWobble() { m_wobbleEnabled = true; }
            void disableWobble() { m_wobbleEnabled = false; }
        private:
            f64 m_wobbleMagnitude = 0.0f;
            bool m_wobbleEnabled = false;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_Camera_h__
