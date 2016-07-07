#include "stdafx.h"
#include "graphics/PostProcess.h"
#include "VorbAssert.hpp"
#include "math/VorbMath.hpp"
#include "graphics/ShaderCommon.inl"
#include "graphics/ShaderManager.h"
#include "graphics/Renderer.h"

vg::FullQuadVBO vg::IPostProcess::quad;

/************************************************************************/
/* IPostProcess                                                         */
/************************************************************************/

void vg::IPostProcess::unregister() {
    vorb_assert(m_renderer, "PostProcess unregistered without having renderer.");

    m_renderer->unregisterPostProcess(this);
}

bool vg::IPostProcess::tryInitQuad() {
    if (quad.isInitialized()) return false;
    quad.init();
    return true;
}

#define TEXTURE_SLOT_INPUT 4
static_assert(TEXTURE_SLOT_INPUT >= (ui32)vg::GBUFFER_TEXTURE_UNITS::COUNT, "PostProcess input will collide with GBuffer");

/************************************************************************/
/* Bloom                                                                */
/************************************************************************/

#define BLOOM_TEXTURE_SLOT_LUMA 5 // texture slot to bind luma texture
#define BLOOM_TEXTURE_SLOT_BLUR 6 // texture slot to bind blur texture=

#pragma region BloomShaderCode

///  Bloom shaders created by Isaque Dutra on 2 June 2015
///  Optimized and ported to Vorb by Ben Arnold on 15 June 2016
///  Copyright 2015 Regrowth Studios
///  All Rights Reserved

const cString BLOOM_LUMA_FRAG_SRC = R"(
///  This fragment shader filters out the image's parts with luma (brightness)
///  stronger than uniform unLumaThresh and scales it proportionally passing it
///  to be blurred by next stages.
///  Taken from OpenGL 4.0 Shading Language Cookbook, First Edition, by David Wolff

// input
in vec2 fUV;
 
// output
out vec4 pColor;

// uniforms
uniform sampler2D unTexColor;   // Texture with rendering color from previous stage
uniform float unLumaThresh;     // Threshold for filtering image luma for bloom bluring

// returns the average brightness of a pixel
float luma(vec3 color) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main() {
    vec3 val = texture(unTexColor, fUV).rgb;
    pColor = vec4(val * clamp(luma(val.rgb) - unLumaThresh, 0.0, 1.0) / (1.0 - unLumaThresh), 1.0);
})";

const cString BLOOM_GAUSS1_FRAG_SRC = R"(
///  This fragment shader implements first stage of two-pass gaussian blur.
///  It blurs only the brighest parts filtered out by the Luma stage.
///  Taken from OpenGL 4.0 Shading Language Cookbook, First Edition, by David Wolff

// input
in vec2 fUV; 

//output
out vec4 pColor;

// uniforms
uniform float unInvHeight;      // 1 / Window height
uniform sampler2D unTexLuma;    // Texture with brighest parts of image
uniform int unGaussianN;        // Radius for gaussian blur
uniform float unWeight[50];     // Gaussian weights

// first pass of gaussian blur, in the y direction
void main() {
    
    vec3 sum = texture(unTexLuma, fUV).rgb * unWeight[0];
    float offset = unInvHeight;
    for(int i = 1; i < unGaussianN; i++) {
        sum += (texture(unTexLuma, vec2(fUV.x, fUV.y + offset)).rgb +
                texture(unTexLuma, vec2(fUV.x, fUV.y - offset)).rgb) * unWeight[i];
        offset += unInvHeight;
    }
    pColor = vec4(sum, 1.0);
}
)";

const cString BLOOM_GAUSS2_FRAG_SRC = R"(
///  This fragment shader implements second stage of two-pass gaussian blur.
///  It blurs on the x-axis the parts already blurred from the first pass,
///  then sums to original color image.
///  Taken from OpenGL 4.0 Shading Language Cookbook, First Edition, by David Wolff

// input
in vec2 fUV; 
// output
out vec4 pColor;

// uniforms
uniform float unInvWidth;       // 1 / Window width
uniform sampler2D unTexColor;   // Original color texture from pass before bloom pass
uniform sampler2D unTexBlur;    // Blur texture from first blur pass
uniform int unGaussianN;        // Radius for gaussian blur
uniform float unWeight[50];     // Gaussian weights

// returns the average brightness of a pixel
float luma(vec3 color) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

// Blurs image on x-axis and sums into the original image
void main() {

    vec3 val = texture(unTexColor, fUV).rgb;
    vec3 sum = texture(unTexBlur, fUV).rgb * unWeight[0];
    float offset = unInvWidth;
    for(int i = 1; i < unGaussianN; i++) {
        sum += (texture(unTexBlur, vec2(fUV.x + offset, fUV.y)).rgb +
                texture(unTexBlur, vec2(fUV.x - offset, fUV.y)).rgb) * unWeight[i];
        offset += unInvWidth;
    }

    pColor = vec4(val + sum, 1.0);
}
)";

#pragma endregion

inline f32 gauss(int i, f32 sigma2) {
    return 1.0 / vmath::sqrt(2 * 3.14159265 * sigma2) * vmath::exp(-(i*i) / (2 * sigma2));
}

void vg::PostProcessBloom::init(ui32 windowWidth, ui32 windowHeight, VGTexture inputColorTexture) {
    m_windowWidth  = windowWidth;
    m_windowHeight = windowHeight;
    m_inputTextures.resize(1, inputColorTexture);
}

void vg::PostProcessBloom::setParams(ui32 gaussianN /* = 20*/, float gaussianVariance /*= 36.0f*/, float lumaThreshold /*= 0.75f*/) {

    vorb_assert(gaussianN <= 50, "Gaussian Radius for PostProcessBloom::setParams has to be less than 50.");

    m_gaussianN = gaussianN;
    m_gaussianVariance = gaussianVariance;
    m_lumaThreshold = lumaThreshold;

    uploadShaderUniforms();
}

void vg::PostProcessBloom::load() {

    vorb_assert(m_windowWidth != 0 && m_windowHeight != 0, "PostProcessBloom was not initialized.");

    tryInitQuad();
    // initialize FBOs
    m_fbos[0].setSize(m_windowWidth, m_windowHeight);
    m_fbos[1].setSize(m_windowWidth, m_windowHeight);
    m_fbos[0].init(vg::TextureInternalFormat::RGBA16F, 0, vg::TextureFormat::RGBA, vg::TexturePixelType::FLOAT);
    m_fbos[1].init(vg::TextureInternalFormat::RGBA16F, 0, vg::TextureFormat::RGBA, vg::TexturePixelType::FLOAT);

    // Load shaders
    // TODO(Ben): Error checking
    // TODO(Ben): Re-use compiled vertex shader
    m_programLuma           = ShaderManager::createProgram(shadercommon::PASSTHROUGH_2D_VERT_SRC, BLOOM_LUMA_FRAG_SRC);
    m_programGaussianFirst  = ShaderManager::createProgram(shadercommon::PASSTHROUGH_2D_VERT_SRC, BLOOM_GAUSS1_FRAG_SRC);
    m_programGaussianSecond = ShaderManager::createProgram(shadercommon::PASSTHROUGH_2D_VERT_SRC, BLOOM_GAUSS2_FRAG_SRC);

    uploadShaderUniforms();
}

void vg::PostProcessBloom::render() {
    // No depth testing
    // TODO(Ben): Don't fuck with existing state. Need state manager.
    glDisable(GL_DEPTH_TEST);

    // luma pass rendering on temporary FBO 1 using the color gbuffer
    m_fbos[0].use();
    glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT);
    glBindTexture(GL_TEXTURE_2D, m_inputTextures.at(0));
    renderStage(m_programLuma);

    // first gaussian blur pass rendering on temporary FBO 2
    m_fbos[1].use();
    glActiveTexture(GL_TEXTURE0 + BLOOM_TEXTURE_SLOT_LUMA);
    m_fbos[0].bindTexture();
    renderStage(m_programGaussianFirst);
    m_fbos[1].unuse(m_windowWidth, m_windowHeight);

    // second gaussian blur pass rendering. Sum initial color with blur color.
    glActiveTexture(GL_TEXTURE0 + BLOOM_TEXTURE_SLOT_BLUR);
    m_fbos[1].bindTexture();

    // Bind output FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFBO);
    renderStage(m_programGaussianSecond);

    // TODO(Ben): Need state manager
    glEnable(GL_DEPTH_TEST);
}

void vg::PostProcessBloom::dispose() {
    m_programLuma.dispose();
    m_programGaussianFirst.dispose();
    m_programGaussianSecond.dispose();
    m_fbos[0].dispose();
    m_fbos[1].dispose();

    // Unregister if we need to.
    if (m_renderer) {
        m_renderer->unregisterPostProcess(this);
    }
}

void vg::PostProcessBloom::uploadShaderUniforms() {
    if (m_programGaussianSecond.isLinked()) {
        m_programLuma.use();
        glUniform1i(m_programLuma.getUniform("unTexColor"), TEXTURE_SLOT_INPUT);
        glUniform1f(m_programLuma.getUniform("unLumaThresh"), m_lumaThreshold);
        m_programLuma.unuse();

        m_programGaussianFirst.use();
        glUniform1i(m_programGaussianFirst.getUniform("unTexLuma"), BLOOM_TEXTURE_SLOT_LUMA);
        glUniform1f(m_programGaussianFirst.getUniform("unInvHeight"), 1.0f / (f32)m_windowHeight);
        glUniform1i(m_programGaussianFirst.getUniform("unGaussianN"), m_gaussianN);
        m_programGaussianFirst.unuse();

        m_programGaussianSecond.use();
        glUniform1i(m_programGaussianSecond.getUniform("unTexColor"), TEXTURE_SLOT_INPUT);
        glUniform1i(m_programGaussianSecond.getUniform("unTexBlur"), BLOOM_TEXTURE_SLOT_BLUR);
        glUniform1f(m_programGaussianSecond.getUniform("unInvWidth"), 1.0f / (f32)m_windowWidth);
        glUniform1i(m_programGaussianSecond.getUniform("unGaussianN"), m_gaussianN);
        m_programGaussianSecond.unuse();

        // Calculate gaussian weights
        f32 weights[50], sum;
        weights[0] = gauss(0, m_gaussianVariance);
        sum = weights[0];
        for (ui32 i = 1; i < m_gaussianN; i++) {
            weights[i] = gauss(i, m_gaussianVariance);
            sum += 2 * weights[i];
        }
        for (ui32 i = 0; i < m_gaussianN; i++) {
            weights[i] = weights[i] / sum;
        }
        m_programGaussianFirst.use();
        glUniform1fv(m_programGaussianFirst.getUniform("unWeight[0]"), m_gaussianN, weights);
        m_programGaussianFirst.unuse();
        m_programGaussianSecond.use();
        glUniform1fv(m_programGaussianSecond.getUniform("unWeight[0]"), m_gaussianN, weights);
        m_programGaussianSecond.unuse();
    }
}

void vg::PostProcessBloom::renderStage(vg::GLProgram& program) {

    program.use();
    program.enableVertexAttribArrays();

    quad.draw();

    program.disableVertexAttribArrays();
    program.unuse();
}

void vg::PostProcessPassthrough::init(VGTexture inputTexture) {
    m_inputTextures.resize(1, inputTexture);
}

void vg::PostProcessPassthrough::load() {
    m_program = ShaderManager::createProgram(shadercommon::PASSTHROUGH_2D_VERT_SRC, shadercommon::TEXTURE_FRAG_SRC);
   
    uploadShaderUniforms();
    tryInitQuad();
}

void vg::PostProcessPassthrough::render() {

    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFBO);

    glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT);
    glBindTexture(GL_TEXTURE_2D, m_inputTextures.at(0));

    m_program.use();
    quad.draw();
    m_program.unuse();


    glEnable(GL_DEPTH_TEST);
}

void vg::PostProcessPassthrough::dispose() {
    m_program.dispose();
}

void vg::PostProcessPassthrough::uploadShaderUniforms() {
    if (m_program.isLinked()) {
        // Can interrupt outer state
        m_program.use();
        glUniform1i(m_program.getUniform("unSampler"), TEXTURE_SLOT_INPUT);
        m_program.unuse();
    }
}