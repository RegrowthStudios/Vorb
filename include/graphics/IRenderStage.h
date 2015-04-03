/// 
///  IRenderStage.h
///  Vorb Engine
///
///  Created by Ben Arnold on 28 Oct 2014
///  Copyright 2014 Regrowth Studios
///  All Rights Reserved
///  
///  This file provides an abstract interface for a render 
///  stage.
///

#pragma once

#ifndef IRenderStage_h_
#define IRenderStage_h_

class Camera; // TODO: We need to implement one

namespace vorb {
    namespace graphics {

        // Forward Decl
        class GLProgram;

        class IRenderStage {
        public:
            IRenderStage(const nString& displayName = "", const Camera* camera = nullptr);
            virtual ~IRenderStage();

            /// Renders the stage
            virtual void render() = 0;

            /// Reloads the shader. By default, it simply
            /// disposes the shader and allows a lazy init at next draw
            virtual void reloadShader();

            /// Disposes and deletes the shader and turns off visibility
            /// If stage does lazy init, shader will reload at next draw
            virtual void dispose();

            /// Sets the visibility of the stage
            /// @param isVisible: The visibility
            virtual void setIsVisible(bool isVisible) {
                m_isVisible = isVisible;
            }

            /// Toggles the isVisible field
            virtual void toggleVisible() {
                m_isVisible = !m_isVisible;
            }

            /// Check if the stage is visible
            virtual const bool& isVisible() const {
                return m_isVisible;
            }

            /// Sets the camera
            /// @param camera: Camera to be used in rendering.
            virtual void setCamera(const Camera* camera) {
                m_camera = camera;
            }

            /// Gets the name
            virtual const nString& getName() const { return m_name; }

        protected:
            GLProgram* m_program = nullptr; ///< Optional shader program
            const Camera* m_camera = nullptr; ///< Optional Camera, not needed for post processing stages
            bool m_isVisible = true; ///< Determines if the stage should be rendered
            nString m_name = ""; ///< Display name of the stage
        };

    }
}
namespace vg = vorb::graphics;

#endif // IRenderStage_h_