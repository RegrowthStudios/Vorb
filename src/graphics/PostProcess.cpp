#include "stdafx.h"
#include "graphics/PostProcess.h"
#include "VorbAssert.hpp"
#include "math/VorbMath.hpp"
#include "graphics/SamplerState.h"
#include "graphics/ShaderCommon.inl"
#include "graphics/ShaderManager.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"

#include <random>


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

#define TEXTURE_SLOT_INPUT0 4
#define TEXTURE_SLOT_INPUT1 5
#define TEXTURE_SLOT_INPUT2 6
#define TEXTURE_SLOT_INPUT3 7
static_assert(TEXTURE_SLOT_INPUT0 >= (ui32)vg::GBUFFER_TEXTURE_UNITS::COUNT, "PostProcess input will collide with GBuffer");

/************************************************************************/
/* Bloom                                                                */
/************************************************************************/

#define BLOOM_TEXTURE_SLOT_LUMA TEXTURE_SLOT_INPUT2 // texture slot to bind luma texture
#define BLOOM_TEXTURE_SLOT_BLUR TEXTURE_SLOT_INPUT3 // texture slot to bind blur texture

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

/************************************************************************/
/* SSAO                                                                 */
/************************************************************************/

#define SSAO_NOISE_TEXTURE_SIZE 4
#define SSAO_SAMPLE_KERNEL_SIZE 32
#define SSAO_BLUR_AMOUNT 2.0f

const cString SSAO_FRAG_SRC = R"(
// Uniforms
uniform sampler2D unTexDepth;
uniform sampler2D unTexNormal;
uniform sampler2D unTexNoise;
const int SAMPLE_KERNEL_SIZE = 32;
uniform vec3 unSampleKernel[SAMPLE_KERNEL_SIZE];
uniform vec2 unNoiseScale;
uniform float unRadius = 1.0;

uniform mat4 unViewMatrix;
uniform mat4 unProjectionMatrix;
uniform mat4 unInvProjectionMatrix;

// Input
in vec2 fUV;

// Output
out float pColor;

vec3 viewSpaceCoordinate(float depth) {
    vec4 screenSpaceCoordinate = vec4(fUV.x * 2.0 - 1.0, fUV.y * 2.0 - 1.0, depth, 1.0);
    screenSpaceCoordinate = unInvProjectionMatrix * screenSpaceCoordinate;
    return screenSpaceCoordinate.xyz / screenSpaceCoordinate.w;
}

// From this http://john-chapman-graphics.blogspot.de/2013/01/ssao-tutorial.html

void main() {
    float depth = texture(unTexDepth, fUV).r;
    vec3 origin = viewSpaceCoordinate(depth);

    vec3 normal = (unViewMatrix * vec4(normalize(texture(unTexNormal, fUV).xyz), 1.0)).xyz;

    // Random sample kernel rotation
    vec3 rotationVector = normalize(vec3(texture(unTexNoise, fUV * unNoiseScale).xy, 0.0));
    vec3 tangent = normalize(rotationVector - normal * dot(rotationVector, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    float totalOcclusion = 0.0;
    for (int i = 0; i < SAMPLE_KERNEL_SIZE; ++i) {
        // Get sample position
        vec3 sample = (tbn * unSampleKernel[i]) * unRadius + origin;

        // Project sample position
        vec4 screenSpaceSample = unProjectionMatrix * vec4(sample, 1.0);
        screenSpaceSample.xy /= screenSpaceSample.w;
        screenSpaceSample.xy = screenSpaceSample.xy * 0.5 + 0.5;
        // Get sample depth
        float sampleDepth = texture(unTexDepth, screenSpaceSample.xy).r;
        // Range check and accumulate
        // TODO(Ben): No branching?
        float rangeCheck = abs(depth - sampleDepth) < unRadius ? 1.0 : 0.0;
        totalOcclusion += (sampleDepth < depth ? 1.0 : 0.0) * rangeCheck;
    }

    pColor = (1.0 - totalOcclusion / float(SAMPLE_KERNEL_SIZE));
}
)";

// TODO(Ben): 2 pass optimization?
const cString SSAO_BLUR_FRAG_SRC = R"(
// Uniforms
uniform sampler2D unTexSSAO;
uniform sampler2D unTexColor;
uniform float unBlurAmount;

// Input
in vec2 fUV;

// Output
out vec4 pColor;
out vec4 pNormal;

void main() {
    float blurSSAO = 0.0f;
    vec2 texelSize = 1.0 / textureSize(unTexSSAO, 0);
    int samples = 0;
    for (float x = fUV.x - texelSize.x * unBlurAmount; x <= fUV.x + texelSize.x * unBlurAmount; x += texelSize.x) {
        for (float y = fUV.y - texelSize.y * unBlurAmount; y <= fUV.y + texelSize.y * unBlurAmount; y += texelSize.y) {
            blurSSAO += texture(unTexSSAO, vec2(x, y)).r;
            samples++;
        }
    }
    blurSSAO /= float(samples);

    vec4 textureColor = texture(unTexColor, fUV);
    pColor = vec4(textureColor.rgb * blurSSAO, textureColor.a);
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
    glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT0);
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
        glUniform1i(m_programLuma.getUniform("unTexColor"), TEXTURE_SLOT_INPUT0);
        glUniform1f(m_programLuma.getUniform("unLumaThresh"), m_lumaThreshold);
        m_programLuma.unuse();

        m_programGaussianFirst.use();
        glUniform1i(m_programGaussianFirst.getUniform("unTexLuma"), BLOOM_TEXTURE_SLOT_LUMA);
        glUniform1f(m_programGaussianFirst.getUniform("unInvHeight"), 1.0f / (f32)m_windowHeight);
        glUniform1i(m_programGaussianFirst.getUniform("unGaussianN"), m_gaussianN);
        m_programGaussianFirst.unuse();

        m_programGaussianSecond.use();
        glUniform1i(m_programGaussianSecond.getUniform("unTexColor"), TEXTURE_SLOT_INPUT0);
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

void vg::PostProcessSSAO::init(ui32 windowWidth, ui32 windowHeight,
                  VGTexture inputColorTexture,
                  VGTexture inputDepthTexture,
                  VGTexture inputNormalTexture,
                  vg::Camera* camera) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    m_inputTextures.resize(3, 0);
    m_inputTextures[0] = inputColorTexture;
    m_inputTextures[1] = inputDepthTexture;
    m_inputTextures[2] = inputNormalTexture;
    m_camera = camera;
}

void vg::PostProcessSSAO::load() {
    std::mt19937 randGenerator;
    std::uniform_real_distribution<f32> range1(-1.0f, 1.0f);
    std::uniform_real_distribution<f32> range2(0.0f, 1.0f);

    // Generate random data
    i32 pixCount = SSAO_NOISE_TEXTURE_SIZE * SSAO_NOISE_TEXTURE_SIZE;
    std::vector<f32v2> data(pixCount);

    for (i32 i = 0; i < pixCount; i++) {
        // TODO(Ben): vec3?
        data[i].x = range1(randGenerator);
        data[i].y = range1(randGenerator);
        data[i] = vmath::normalize(data[i]);
    }

    // Build noise texture
    glGenTextures(1, &m_noiseTexture);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, SSAO_NOISE_TEXTURE_SIZE, SSAO_NOISE_TEXTURE_SIZE, 0, GL_RG, GL_FLOAT, data.data());
    vg::SamplerState::POINT_WRAP.set(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_ssaoTarget.setSize(m_windowWidth, m_windowHeight);
    m_ssaoTarget.init(vg::TextureInternalFormat::R32F);

    m_sampleKernel.resize(SSAO_SAMPLE_KERNEL_SIZE);
    for (unsigned int i = 0; i < SSAO_SAMPLE_KERNEL_SIZE; i++) {
        m_sampleKernel[i] = vmath::normalize(f32v3(range1(randGenerator),
            range1(randGenerator),
            range2(randGenerator)));
        // Use accelerating interpolation
        f32 scale = (f32)i / (f32)SSAO_SAMPLE_KERNEL_SIZE;
        scale = lerp(0.1f, 1.0f, scale * scale);
        m_sampleKernel[i] *= scale;
    }

    m_programSSAO = ShaderManager::createProgram(shadercommon::PASSTHROUGH_2D_VERT_SRC, SSAO_FRAG_SRC);
    m_programBlur = ShaderManager::createProgram(shadercommon::PASSTHROUGH_2D_VERT_SRC, SSAO_BLUR_FRAG_SRC);

    uploadShaderUniforms();
}

void vg::PostProcessSSAO::render() {
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    const f32m4& projectionMatrix = m_camera->getProjectionMatrix();
    const f32m4& viewMatrix = m_camera->getViewMatrix();

    { // SSAO pass      
        m_ssaoTarget.use();
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind textures
        glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT0);
        glBindTexture(GL_TEXTURE_2D, m_inputTextures.at(1));
        glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT1);
        glBindTexture(GL_TEXTURE_2D, m_inputTextures.at(2));
        glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT2);
        glBindTexture(GL_TEXTURE_2D, m_noiseTexture);

        m_programSSAO.use();

        glUniformMatrix4fv(m_programSSAO.getUniform("unViewMatrix"), 1, false, &viewMatrix[0][0]);
        glUniformMatrix4fv(m_programSSAO.getUniform("unProjectionMatrix"), 1, false, &projectionMatrix[0][0]);
        glUniformMatrix4fv(m_programSSAO.getUniform("unInvProjectionMatrix"), 1, false, &vmath::inverse(projectionMatrix)[0][0]);

        quad.draw();
    }

    { // Blur pass
        glBindFramebuffer(GL_FRAMEBUFFER, m_outputFBO);
        glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT0);
        glBindTexture(GL_TEXTURE_2D, m_inputTextures.at(0));
        glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT1);
        glBindTexture(GL_TEXTURE_2D, m_ssaoTarget.getTextureID());

        m_programBlur.use();

        quad.draw();

        vg::GLProgram::unuse();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}

void vg::PostProcessSSAO::dispose() {
    if (m_noiseTexture) {
        glDeleteTextures(1, &m_noiseTexture);
        m_noiseTexture = 0;
    }
    m_programSSAO.dispose();
    m_ssaoTarget.dispose();
}

void vg::PostProcessSSAO::uploadShaderUniforms() {
    m_programSSAO.use();
    glUniform1i(m_programSSAO.getUniform("unTexDepth"), TEXTURE_SLOT_INPUT0);
    glUniform1i(m_programSSAO.getUniform("unTexNormal"), TEXTURE_SLOT_INPUT1);
    glUniform1i(m_programSSAO.getUniform("unTexNoise"), TEXTURE_SLOT_INPUT2);
    glUniform3fv(glGetUniformLocation(m_programSSAO.getID(), "unSampleKernel"), m_sampleKernel.size(), &m_sampleKernel.data()->x);
    glUniform2f(m_programSSAO.getUniform("unNoiseScale"),
                (f32)m_ssaoTarget.getWidth() / SSAO_NOISE_TEXTURE_SIZE,
                (f32)m_ssaoTarget.getHeight() / SSAO_NOISE_TEXTURE_SIZE);

    m_programBlur.use();
    glUniform1i(m_programBlur.getUniform("unTexColor"), TEXTURE_SLOT_INPUT0);
    glUniform1i(m_programBlur.getUniform("unTexSSAO"), TEXTURE_SLOT_INPUT1);
    glUniform1f(m_programBlur.getUniform("unBlurAmount"), SSAO_BLUR_AMOUNT);
    m_programBlur.unuse();
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

    glActiveTexture(GL_TEXTURE0 + TEXTURE_SLOT_INPUT0);
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
        glUniform1i(m_program.getUniform("unSampler"), TEXTURE_SLOT_INPUT0);
        m_program.unuse();
    }
}
