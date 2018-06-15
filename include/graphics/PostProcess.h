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
    class Camera;

    // TODO(Ben): Visual shader programming tool with live recompiling.
    // TODO(Ben): FBO Cacheing.

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

        /*! @brief Gets and sets the input textures.
         * Derived classes must allocate input texture space in m_inputTextures or setInputTexture will crash.
         */
        virtual void setInputTexture (ui32 index, VGTexture  inputTexture)  { m_inputTextures.at(index) = inputTexture; }
        virtual void setInputTextures(std::vector<VGTexture> inputTextures) { m_inputTextures           = inputTextures; }
        virtual const std::vector<VGTexture>& getInputTextures() const { return m_inputTextures; }

        /*! @brief Gets and sets the input textures.
         * It is assumed that the outputFBO is a format that works for this PostProcess. 
         */
        virtual void          setOutputFBO(VGFramebuffer outputFBO) { m_outputFBO = outputFBO; }
        virtual VGFramebuffer getOutputFBO() const                  { return m_outputFBO; }

        /*! @brief Checks if quad is created, and creates it if not.
         *  This must be called to initialize the shared quad. Recommend calling in
         *  any shader that needs the quad.
         *  @return False if the quad is already initialized.
         */
        static bool tryInitQuad();

        static vg::FullQuadVBO quad; ///< Optional shared VBO. Call dispose manually.

    protected:

        Renderer* m_renderer = nullptr; ///< Renderer that owns this.

        std::vector<VGTexture> m_inputTextures; ///< Optional input textures, some PostProcesses may not use.

        VGFramebuffer m_outputFBO    = 0; ///< FBO that we will write to.
    };

    /************************************************************************/
    /* Bloom                                                                */
    /************************************************************************/
    /*! @brief Renders the bloom post process: A gaussian blur of bright objects.
     * 
     *  @input Texture 0 as color.
     *  @output[0] Color.
     */
    class PostProcessBloom : public IPostProcess {
    public:
        void         init     (ui32 windowWidth, ui32 windowHeight, VGTexture inputColorTexture);
        void         setParams(ui32 gaussianN = 20, f32 gaussianVariance = 36.0f, f32 lumaThreshold = 0.75f);
        void         load     () override;
        virtual void render   () override;
        virtual void dispose  () override;

    private:
        virtual void uploadShaderUniforms();

        void renderStage (vg::GLProgram& program);

        /// Shaders
        vg::GLProgram m_programLuma;
        vg::GLProgram m_programGaussianFirst;
        vg::GLProgram m_programGaussianSecond;

        vg::GLRenderTarget m_fbos[2];

        ui32 m_windowWidth  = 0;
        ui32 m_windowHeight = 0;

        /// Parameters
        ui32 m_gaussianN        = 20;     ///< Threshold for filtering image luma for bloom bluring
        f32  m_gaussianVariance = 36.0f;  ///< Radius number for gaussian blur. Must be less than 50.
        f32  m_lumaThreshold    = 0.75f;  ///< Gaussian variance for blur pass
    };

    /************************************************************************/
    /* SSAO                                                                */
    /************************************************************************/
    /*! @brief Renders the SSAO post process: Screen space ambient occlusion.
    */
    class PostProcessSSAO : public IPostProcess {
    public:
        void         init(ui32 windowWidth, ui32 windowHeight,
                          VGTexture inputColorTexture,
                          VGTexture inputDepthTexture,
                          VGTexture inputNormalTexture,
                          vg::Camera* camera);

        void         load() override;
        virtual void render() override;
        virtual void dispose() override;

    private:
        virtual void uploadShaderUniforms();
   
        /// Shaders
        vg::GLProgram m_programSSAO;
        vg::GLProgram m_programBlur;

        vg::Camera* m_camera = nullptr;

        VGTexture m_noiseTexture;

        vg::GLRenderTarget m_ssaoTarget;

        ui32 m_windowWidth = 0;
        ui32 m_windowHeight = 0;

        std::vector<f32v3> m_sampleKernel;
    };

    /************************************************************************/
    /* Passthrough                                                          */
    /************************************************************************/
    /*! @brief Simply renders a 2D texture from one FBO to another.
    *
    * @input Texture.
    * @output[0] Texture.
    */
    class PostProcessPassthrough : public IPostProcess {
    public:
        void         init(VGTexture inputTexture);
        virtual void load()    override;
        virtual void render()  override;
        virtual void dispose() override;
        virtual void uploadShaderUniforms();

    private:
        vg::GLProgram m_program;
    };

    // TODO(Ben): Gaussian blur.
}
}
namespace vg = vorb::graphics;

#endif // !Vorb_PostProcess_h__

