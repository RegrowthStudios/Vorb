/// 
///  RenderPipeline.h
///  Vorb Engine
///
///  Created by Ben Arnold on 28 Oct 2014
///  Copyright 2014 Regrowth Studios
///  All Rights Reserved
///  
///  Summary:
///  This file provides an interface for a render pipeline.
///

#pragma once

#ifndef RenderPipeline_h__
#define RenderPipeline_h__

#include <vector>
#include <memory>
#include "../ui/GameWindow.h"

class Camera;

namespace vorb {
    namespace graphics {

        class IRenderStage;

        /// This class implements a rendering pipeline.
        /// For basic rendering, just create the pipeline, add stages,
        /// set a camera, and call render.
        /// Derive from RenderPipeline to customize the rendering process for
        /// adding things such as post processing.
        class RenderPipeline {
        public:
            virtual void init(vui::GameWindow* window);
            /// Frees all resources and disposes all render stages
            virtual void dispose();

            /// Renders the pipeline
            virtual void render();

            /// Adds a stage to the internal stack of stages
            /// Also sets its camera if the render pipeline has a camera
            /// @param stage: The render stage to add
            /// @return handle to the stage
            void registerStage(IRenderStage* stage);

            /// Sets the camera for the render stages.
            /// Overwrites any existing cameras for tracked stages.
            /// @param Camera: Camera to set
            virtual void setCamera(Camera* camera);

            /// Gets the current camera
            virtual const Camera* getCamera() const { return m_camera; }
        protected:
            vui::GameWindow* m_window = nullptr;
            Camera* m_camera = nullptr; ///< Optional camera for rendering.
            std::vector<IRenderStage*> m_stages; ///< List of all stages
        };
    }
}
namespace vg = vorb::graphics;

#endif // RenderPipeline_h__
