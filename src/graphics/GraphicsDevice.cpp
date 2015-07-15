#include "stdafx.h"
#include "graphics/GraphicsDevice.h"

#ifndef VORB_USING_PCH
#include <GL/glew.h>

#include "compat.h"
#endif // !VORB_USING_PCH

#if defined(VORB_IMPL_UI_SDL)
#if defined(OS_WINDOWS)
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#elif defined(VORB_IMPL_UI_GLFW)
#include <GLFW/glfw3.h>
#elif defined(VORB_IMPL_UI_SFML)
#include <SFML/Window/VideoMode.hpp>
#endif


vg::GraphicsDevice::GraphicsDevice() :
_props({}) {
    // Empty
}

void vg::GraphicsDevice::refreshInformation() {
    // Whenever Information Is Refreshed, The Current Device Is Refreshed
    _current = this;

    // Get Display Information
#if defined(VORB_IMPL_UI_SDL)
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    _props.nativeScreenWidth = displayMode.w;
    _props.nativeScreenHeight = displayMode.h;
    _props.nativeRefreshRate = displayMode.refresh_rate;
#elif defined(VORB_IMPL_UI_GLFW)
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    auto& displayMode = *glfwGetVideoMode(monitor);
    _props.nativeScreenWidth = displayMode.width;
    _props.nativeScreenHeight = displayMode.height;
    _props.nativeRefreshRate = displayMode.refreshRate;
#elif defined(VORB_IMPL_UI_SFML)
    auto displayMode = sf::VideoMode::getDesktopMode();
    _props.nativeScreenWidth = displayMode.width;
    _props.nativeScreenHeight = displayMode.height;
    _props.nativeRefreshRate = 60; // This is what Laurent says about monitors, so it must be true...
#endif

    // Get The OpenGL Implementation Information
    _props.glVendor = (const cString)glGetString(GL_VENDOR);
    _props.glVersion = (const cString)glGetString(GL_VERSION);
    glGetIntegerv(GL_MAJOR_VERSION, &_props.glVersionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &_props.glVersionMinor);
    _props.glslVersion = (const cString)glGetString(GL_SHADING_LANGUAGE_VERSION);

    // Get Vertex Information
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &_props.maxVertexAttributes);

    // Get MSAA Information
    glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &_props.maxColorSamples);
    glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &_props.maxDepthSamples);

    // Get Texture Unit Information
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_props.maxTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_props.maxTextureSize);
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &_props.max3DTextureSize);
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &_props.maxArrayTextureLayers);

#ifdef DEBUG
    printf("Graphics Device Information Refreshed:\n");

    printf("\n=== OpenGL Implementation ===\n");
    printf("Vendor:                   %s\n", _props.glVendor);
    printf("GL Version:               %s\n", _props.glVersion);
    printf("GL Version (Strict):      %d.%d\n", _props.glVersionMajor, _props.glVersionMinor);
    printf("GLSL Version:             %s\n", _props.glslVersion);

    printf("\n=== Vertex Properties ===\n");
    printf("Max Vertex Attributes:    %d\n", _props.maxVertexAttributes);

    printf("\n=== Texture Properties ===\n");
    printf("Max Texture Units:        %d\n", _props.maxTextureUnits);
    printf("Max Texture Size:         %d\n", _props.maxTextureSize);
    printf("Max 3D Texture Size:      %d\n", _props.max3DTextureSize);
    printf("Max Array Texture Layers: %d\n", _props.maxArrayTextureLayers);
#endif // DEBUG
}

void vg::GraphicsDevice::initResolutions(void* w) {
#if defined(VORB_IMPL_UI_SDL)
    i32 dispIndex = SDL_GetWindowDisplayIndex((SDL_Window*)w);
    i32 dispCount = SDL_GetNumDisplayModes(dispIndex);
    SDL_DisplayMode dMode;
    for (i32 dmi = 0; dmi < dispCount; dmi++) {
        SDL_GetDisplayMode(dispIndex, dmi, &dMode);
        _props.resolutionOptions.push_back(ui32v2(dMode.w, dMode.h));
    }
#elif defined(VORB_IMPL_UI_GLFW)
    int dispCount;
    GLFWmonitor* monitor = glfwGetWindowMonitor((GLFWwindow*)w);
    if (!monitor) monitor = glfwGetPrimaryMonitor();
    auto dmodes = glfwGetVideoModes(monitor, &dispCount);
    for (i32 dmi = 0; dmi < dispCount; dmi++) {
        _props.resolutionOptions.push_back(ui32v2(dmodes[dmi].width, dmodes[dmi].height));
    }
#elif defined(VORB_IMPL_UI_SFML)
    auto modes = sf::VideoMode::getFullscreenModes();
    for (auto& mode : modes) _props.resolutionOptions.push_back(ui32v2(mode.width, mode.height));
#endif

    // Sort display modes
    std::sort(_props.resolutionOptions.begin(), _props.resolutionOptions.end(), [] (const ui32v2& r1, const ui32v2& r2) {
        if (r1.x == r2.x) return r1.y > r2.y;
        else return r1.x > r2.x;
    });
    auto iter = std::unique(_props.resolutionOptions.begin(), _props.resolutionOptions.end());
    _props.resolutionOptions.resize(iter - _props.resolutionOptions.begin());
}

vg::GraphicsDevice* vg::GraphicsDevice::_current = nullptr;
