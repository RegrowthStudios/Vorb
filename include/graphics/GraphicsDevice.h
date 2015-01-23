///
/// GraphicsDevice.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 22 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Stores information about the graphics device
///

#pragma once

#ifndef GraphicsDevice_h__
#define GraphicsDevice_h__

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
            GraphicsDevice(void* w);

            static GraphicsDevice* getCurrent() {
                return _current;
            }

            void refreshInformation();

            const GraphicsDeviceProperties& getProperties() const {
                return _props;
            }
        private:
            void initResolutions(void* w);

            GraphicsDeviceProperties _props;

            static GraphicsDevice* _current;
        };
    }
}
namespace vg = vorb::graphics;

#endif // GraphicsDevice_h__
