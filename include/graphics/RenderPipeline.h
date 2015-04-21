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
            RenderPipeline();
            virtual ~RenderPipeline();

            typedef std::shared_ptr<IRenderStage> StagePtr;

            /// Sets the camera for the render stages.
            /// Overwrites any existing cameras for tracked stages.
            /// @param Camera: Camera to set
            virtual void setCamera(Camera* camera);

            /// Adds a stage to the internal stack of stages
            /// Also sets its camera if the render pipeline has a camera
            /// @param stage: The render stage to add
            /// @return handle to the stage
            IRenderStage* addStage(StagePtr stage);

            /// Renders the pipeline
            virtual void render();

            /// Frees all resources and disposes all render stages
            /// @param shouldDisposeStages: Set to true when you want to
            /// dispose managed stages as well.
            virtual void destroy(bool shouldDisposeStages);

            /// Disposes all render stages
            /// Call before destroy
            virtual void disposeStages();

            /// Reloads all shaders
            virtual void reloadShaders();
        protected:
            Camera* m_camera = nullptr; ///< Optional camera for rendering.
            std::vector <StagePtr> m_stages; ///< List of all stages
        };
    }
}
namespace vg = vorb::graphics;

#endif // RenderPipeline_h__