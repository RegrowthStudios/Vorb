//
// Camera.h
// Vorb Engine
//
// Created by Benjamin Arnold on 20 Jun 2016
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Camera.h
* @brief 
* Basic 3D camera implementation.
*
*/

#pragma once

#ifndef Vorb_Camera_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Camera_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Frustum.h"

namespace vorb     {
namespace graphics {

    class Camera {
    public:
        Camera();
        virtual ~Camera();

        /*! @brief Sets up the camera.
         */
        virtual void init(f32 aspectRatio);

        /*! @brief Updates the camera matrices. Call every frame before render.
         */
        virtual void update();

        /*! @brief Check if a point is in the camera frustum.
         */
        bool pointInFrustum (const f32v3& pos)             const { return m_frustum.pointInFrustum(pos); }
        /*! @brief Check if a sphere is in the camera frustum.
        */
        bool sphereInFrustum(const f32v3& pos, f32 radius) const { return m_frustum.sphereInFrustum(pos, radius); }

        /************************************************************************/
        /* Mutators                                                             */
        /************************************************************************/
        /*! @brief Translate the camera.
         */
        void offsetPosition(const f32v3& offset);
        void offsetPosition(const f64v3& offset);
        /*! @brief Apply a quaternion rotation to the camera.
         */
        virtual void rotate(const f32q& rot);
        /*! @brief Rotate with pitch and yaw.
        */
        virtual void rotate(f32 yaw, f32 pitch);
        /*! @brief Rotate with pitch and yaw in an FPS style with (0, 1, 0) as the UP vector.
         *  @param clampVerticalRotation: Set to true to not allow rotation beyond absolute up or down.
         */
        virtual void rotateAbsoluteUp(f32 yaw, f32 pitch, bool clampVerticalRotation = false);
        
        /*! @brief Apply roll rotation only.
         */
        virtual void roll(f32 roll);

        void setOrientation  (const f64q& orientation);
        void setFocalPoint   (const f64v3& focalPoint) { m_focalPoint = focalPoint; m_viewChanged = true; }
        void setPosition     (const f64v3& position)   { m_focalPoint = m_position = position; m_focalLength = 0;  m_viewChanged = true; }
        void setDirection    (const f32v3& direction)  { m_direction = direction; m_viewChanged = true; }
        void setRight        (const f32v3& right)      { m_right = right; m_viewChanged = true; }
        void setUp           (const f32v3& up)         { m_up = up; m_viewChanged = true; }
        void setClippingPlane(f32 zNear, f32 zFar)     { m_zNear = zNear; m_zFar = zFar; m_projectionChanged = true; }
        void setFieldOfView  (f32 fieldOfView)         { m_fieldOfView = fieldOfView; m_projectionChanged = true; }
        void setFocalLength  (f32 focalLength)         { m_focalLength = focalLength; m_viewChanged = true; }
        void setAspectRatio  (f32 aspectRatio)         { m_aspectRatio = aspectRatio; m_projectionChanged = true; }

        /************************************************************************/
        /* Accessors                                                            */
        /************************************************************************/
        const f64v3& getPosition      () const { return m_position;       }
        const f64v3& getFocalPoint    () const { return m_focalPoint;     }
        const f64&   getFocalLength   () const { return m_focalLength;    }
        const f64&   getMaxFocalLength() const { return m_maxFocalLength; }

        const f32v3& getDirection() const { return m_direction; }
        const f32v3& getRight    () const { return m_right;     }
        const f32v3& getUp       () const { return m_up;        }

        const f32m4& getProjectionMatrix    () const { return m_projectionMatrix;     }
        const f32m4& getViewMatrix          () const { return m_viewMatrix;           }
        const f32m4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

        const f32& getNearClip   () const { return m_zNear;       }
        const f32& getFarClip    () const { return m_zFar;        }
        const f32& getFieldOfView() const { return m_fieldOfView; }
        const f32& getAspectRatio() const { return m_aspectRatio; }

        const Frustum& getFrustum() const { return m_frustum; }
    private:
        void updateView      ();
        void updateProjection();

        f64v3 m_focalPoint     = f64v3(0.0);
        f64v3 m_position       = f64v3(0.0);
        f64   m_focalLength    = 0.0;
        f64   m_maxFocalLength = 0.0;
        
        f32 m_zNear       = 0.1f;
        f32 m_zFar        = 10000.0f;
        f32 m_fieldOfView = 75.0f;
        f32 m_aspectRatio = 0.0f;

        f32v3 m_direction = f32v3(1.0f, 0.0f, 0.0f);
        f32v3 m_right     = f32v3(0.0f, 0.0f, 1.0f);
        f32v3 m_up        = f32v3(0.0f, 1.0f, 0.0f);

        f32m4 m_projectionMatrix;
        f32m4 m_viewMatrix;
        f32m4 m_viewProjectionMatrix;

        bool m_viewChanged       = true;
        bool m_projectionChanged = true;

        Frustum m_frustum;

        static const f32v3 UP_ABSOLUTE;
    };

}
}
namespace vg = vorb::graphics;

#endif // !Vorb_Camera_h__

