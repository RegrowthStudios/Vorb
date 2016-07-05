//
// Renderer.h
// Vorb Engine
//
// Created by Benjamin Arnold on 22 Jun 2016
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Renderer.h
* @brief Handles rendering of scenes and post processes to a vui::GameWindow.
*
*
*/

#pragma once

#ifndef Vorb_Renderer_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Renderer_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "../ui/GameWindow.h"
#include "../VorbPreDecl.inl"

DECL_VUI(struct GameTime);

namespace vorb {
    namespace graphics {

        enum class GBUFFER_TEXTURE_UNITS {
            COLOR = 0,
            POSITION,
            NORMAL,
            COUNT
        };

        // Forward Declare
        class GBuffer;
        class IScene;
        class IPostProcess;
        class PostProcessPassthrough;

        /// Manages storage and rendering of scenes and post processes
        class Renderer {
        public:
            Renderer();
            virtual ~Renderer();

            /*! @brief Initializes the SceneRenderer for the GameWindow.
             *  Do not call twice without first destroying.
             */
            virtual bool init(vui::GameWindow* window);

            /*! @brief Loads any scenes or postProcessed that need to be loaded.
             */
            virtual void load();

            /*! @brief Registers a scene for load and then rendering.
             */
            virtual void registerScene(IScene* scene);

            /*! @brief Unregisters a scene for rendering.
             *  Does not dispose the scene.
             *  @return true on success.
             */
            virtual bool unregisterScene(IScene* scene);

            /*! @brief Registers a PostProcess for load and then rendering.
            */
            virtual void registerPostProcess(IPostProcess* postProcess);

            /*! @brief Unregisters a PostProcess for rendering.
            *  Does not dispose the PostProcess.
            *  @return true on success.
            */
            virtual bool unregisterPostProcess(IPostProcess* postProcess);

            /*! @Brief sets background color for window
             */
            virtual void setBackgroundColor(const color4& color);
            virtual void setBackgroundColor(const f32v4& color);

            /*! @Brief clears depth and color buffer and sets viewport.
             */
            virtual void beginRenderFrame();

            /*! @Brief Sync the window and display to the screen.
            */
            virtual void sync(ui32 frameTime) { m_window->sync(frameTime); }

            /*! @brief Renders all scenes in order that they were registered
             */
            virtual void renderScenes(const vui::GameTime& gameTime);

            /*! @brief Renders all postprocess stages. Call after renderScenes.
             *  You must call this even if you don't use post processing in order to render the gbuffer.
             */
            virtual void renderPostProcesses();

            /*! @brief Cleans up the renderer and disposes all scenes and postprocesses.
             */
            virtual void dispose();

            /*!
             */

            /************************************************************************/
            /* Accessors                                                            */
            /************************************************************************/
            const std::vector<IScene*>&       getScenes()        const { return m_scenes; }
            const std::vector<IPostProcess*>& getPostProcesses() const { return m_postProcesses; }
            bool                              isInitialized()    const { return m_isInitialized; }

        protected:
            bool             m_isInitialized    = false;
            vui::GameWindow* m_window           = nullptr; ///< The window we are targeting.

            std::unique_ptr<PostProcessPassthrough> m_postProcessPassthrough = nullptr; ///< Default post process when none are active

            // TODO(Ben): FBO Cache
            std::unique_ptr<vg::GBuffer> m_gBuffer;

            std::vector<IScene*>       m_scenes;                 ///< Scenes that are loaded and ready to render
            std::vector<IScene*>       m_sceneLoadQueue;         ///< Scenes that need to load
            std::vector<IPostProcess*> m_postProcesses;          ///< PostPorcesses that are loaded and ready to render
            std::vector<IPostProcess*> m_postProcessesLoadQueue; ///< PostProcesses that need to load
        };
    }
}
namespace vg = vorb::graphics;


#endif // !Vorb_Renderer_h__

