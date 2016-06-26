//
// Scene.h
// Vorb Engine
//
// Created by Benjamin Arnold on 22 Jun 2016
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Scene.h
* @brief A complete graphical scene for rendering to the screen.
*
*
*/

#pragma once

#ifndef Vorb_Scene_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Scene_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "../VorbPreDecl.inl"

DECL_VUI(struct GameTime);

namespace vorb {
    namespace graphics {

        // Forward Declare
        class Camera;
        class GLRenderTarget;
        class Renderer;

        /// A scene, complete with its own camera and rendering logic.
        class IScene {
        public:
            friend class Renderer;

            IScene();
            virtual ~IScene();

            // You should implement your own initialization logic

            /*! @brief Sets up the camera.
             * Override this to provide a custom camera implementation.
             */
            virtual void initCamera();

            /* @brief Load all graphics objects.
             *  Gets called by SceneRenderer.
             */
            virtual void load() = 0;

            /*! @brief Renders a scene.
             *
             */
            virtual void render(const vui::GameTime& gameTime) = 0;

            /*! @brief Unregisters from the SceneRenderer
             */
            virtual void unregister();

            /*! @brief Frees any resources and resets state.
            */
            virtual void dispose();

            Camera* getCamera() { return m_camera.get(); }

        protected:
            std::unique_ptr<Camera> m_camera   = nullptr;
            Renderer*               m_renderer = nullptr; ///< Renderer that owns this.
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_Scene_h__

