//
// GraphicsDevice.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file GraphicsDevice.h
 * @brief Stores information about the graphics device.
 */

#pragma once

#ifndef Vorb_GraphicsDevice_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GraphicsDevice_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH
#include <vector>

namespace vorb {
    namespace graphics {
        struct GraphicsDeviceProperties {
        public:
            i32 maxColorSamples;
            i32 maxDepthSamples;

            i32 maxVertexAttributes;

            i32 maxTextureUnits;
            i32 maxTextureSize;
            i32 max3DTextureSize;
            i32 maxArrayTextureLayers;

            i32 nativeScreenWidth;
            i32 nativeScreenHeight;
            i32 nativeRefreshRate;
            std::vector<ui32v2> resolutionOptions;

            const cString glVendor;
            const cString glVersion;
            i32 glVersionMajor;
            i32 glVersionMinor;
            const cString glslVersion;
        };

        class GraphicsDevice {
        public:
            GraphicsDevice();

            static GraphicsDevice* getCurrent() {
                if (!_current) {
                    _current = new GraphicsDevice();
                    _current->refreshInformation();
                }
                return _current;
            }

            void initResolutions(void* w);
            void refreshInformation();

            const GraphicsDeviceProperties& getProperties() const {
                return _props;
            }
        private:
            GraphicsDeviceProperties _props;

            static GraphicsDevice* _current;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_GraphicsDevice_h__
