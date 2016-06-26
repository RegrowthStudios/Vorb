//
// PostProcess.h
// Vorb Engine
//
// Created by Benjamin Arnold on 22 Jun 2016
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file PostProcess.h
* @brief A PostProcess stage for processing a drawn FBO.
*
*
*/

#pragma once

#ifndef Vorb_PostProcess_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_PostProcess_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "FullQuadVBO.h"
#include "GLProgram.h"
#include "GLRenderTarget.h"

namespace vorb {
namespace graphics {

    // Forward Declare
    class GLRenderTarget;
    class Renderer;

    /************************************************************************/
    /* Base                                                                 */
    /************************************************************************/
    class IPostProcess {
    public:
        friend class Renderer;

        /*! @brief Loads shaders and other graphical objects.
         *  Gets called by SceneRenderer.
         */
        virtual void load() = 0;

        /*! @brief Renders the post process
         *  Gets called by SceneRenderer.
         */
        virtual void render() = 0;

        /*! @brief Frees resources.
         */
        virtual void dispose() = 0;

        /*! @brief Unregisters from the SceneRenderer
        */
        virtual void unregister();

    protected:
        Renderer* m_renderer = nullptr; ///< Renderer that owns this.
    };

    /************************************************************************/
    /* Bloom                                                                */
    /************************************************************************/
    class PostProcessBloom : public IPostProcess {
    public:
        void         init     (ui32 windowWidth, ui32 windowHeight);
        void         setParams(ui32 gaussianN = 20, f32 gaussianVariance = 36.0f, f32 lumaThreshold = 0.75f);
        void         load     () override;
        virtual void render   () override;
        virtual void dispose  () override;

    private:
        void renderStage   (vg::GLProgram& program);
        void uploadUniforms();

        /// Shaders
        vg::GLProgram m_programLuma;
        vg::GLProgram m_programGaussianFirst;
        vg::GLProgram m_programGaussianSecond;

        vg::FullQuadVBO m_quad;

        vg::GLRenderTarget m_fbos[2];

        ui32 m_windowWidth  = 0;
        ui32 m_windowHeight = 0;

        /// Parameters
        ui32 m_gaussianN        = 20;     ///< Threshold for filtering image luma for bloom bluring
        f32  m_gaussianVariance = 36.0f;  ///< Radius number for gaussian blur. Must be less than 50.
        f32  m_lumaThreshold    = 0.75f;  ///< Gaussian variance for blur pass
    };
}
}
namespace vg = vorb::graphics;

#endif // !Vorb_PostProcess_h__

