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

#include "GLProgram.h"

class Camera; // TODO: We need to implement one

namespace vorb {
    namespace graphics {

        class IRenderStage {
        public:
            /// Renders the stage
            virtual void render(const Camera* camera) = 0;

            virtual void init() = 0;

            virtual void dispose() = 0;

            /// Sets the visibility of the stage
            /// @param isVisible: The visibility
            virtual void setActive(bool isVisible) { m_isActive = isVisible; }

            /// Toggles the isVisible field
            virtual void toggleActive() { m_isActive = !m_isActive; }

            /// Check if the stage is visible
            virtual const bool& isActive() const { return m_isActive; }
        protected:
            bool m_isActive = true; ///< Determines if the stage should be rendered
        };
    }
}
namespace vg = vorb::graphics;

#endif // IRenderStage_h_
