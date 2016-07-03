//
// ResourceDescriptions.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file ResourceDescriptions.h
 * 
 * \brief 
 */

#pragma once

#ifndef Vorb_ResourceDescriptions_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ResourceDescriptions_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "GLEnums.h"
#include "VGEnums.h"
#include "../utils.h"

namespace vorb {
    namespace graphics {
        struct ShaderCompilerInfo {
        public:
            struct {
                ui32 major;
                ui32 minor;
            } version;
            

            // TODO(Cristian): Add other compilation options
        };

        struct ShaderBytecode {
        public:
            ShaderType type;
            void* code;
            size_t length;

            void alloc(size_t l) {
                code = malloc(l);
            }
            void free() {
                ::free(code);
            }
        };

        struct VertexElementDescription {
        public:
            ui32 slot;
            vg::Semantic semantic;
            ui32 semanticIndex;
            ui32 offset;
            i32 size;
            vg::MemoryFormat type;
        };

        struct VertexDescription {
        private:
            VertexElementDescription* m_elements;
            size_t m_numElements;
            ui32 m_id;
        };

        struct InitalResourceData {
        public:
            void* data;
            struct {
                UNIT_SPACE(BYTES) size_t texture2DRow; ///< The distance of the first two pixels between rows (y) in a 2D and 3D texture
                UNIT_SPACE(BYTES) size_t texture3DSlice; ///< The distance of the first two pixels between slices (z) in a 3D texture
            } stride; ///< Pixel stride information for variable-packed texture data (leave values as 0 if it is tightly packed)
        };

        struct BufferDescription {
        public:
            vg::BufferTarget type;
            vg::BufferUsageHint usage;
            vg::MemoryFormat format;
            size_t structSize;
            size_t size;
        };

        struct ConstantBlockDescription {
        public:
            size_t size;
        };

        struct Texture1DDescription {
        public:
            union {
                size_t dimensions[1];
                struct {
                    size_t width;
                };
            };
            size_t atlasPages;
            vg::MemoryFormat format;
        };

        struct Texture2DDescription {
        public:
            union {
                size_t dimensions[2];
                struct {
                    size_t width;
                    size_t height;
                };
            };
            size_t atlasPages;
            vg::MemoryFormat format;
        };

        struct Texture3DDescription {
        public:
            union {
                size_t dimensions[3];
                struct {
                    size_t width;
                    size_t height;
                    size_t depth;
                };
            };
            vg::MemoryFormat format;
        };

        struct BlendStateDescription {
        public:
            struct Function {
                BlendOperation operation;
                BlendMultiplier source;
                BlendMultiplier destination;
            };
            struct TargetOp {
                bool enableBlending;
                Function color;
                Function alpha;
                union {
                    struct {
                        bool red : 1;
                        bool green : 1;
                        bool blue : 1;
                        bool alpha : 1;
                    } channel;
                    ui8 mask;
                } write;
            };

            bool useAlphaCoverage;
            bool useMultipleBlending;
            union {
                struct {
                    TargetOp renderTargetOp;
                    TargetOp auxiliaryTargets[7];
                };
                TargetOp renderTargetOps[8];
            };
        };

        struct DepthStencilStateDescription {
        public:
            struct FaceOps {
                ComparisonMode comparisonFunction;
                StencilOperation stencilFail;
                StencilOperation stencilPassDepthFail;
                StencilOperation pass;
            };

            bool enableDepth;
            bool writeDepth;
            ComparisonMode depthFunction;

            bool enableStencil;
            struct {
                FaceOps front;
                FaceOps back;
            } faceOps;
            ui8 stencilReadMask;
            ui8 stencilWriteMask;
        };

        struct RasterizerStateDescription {
        public:
            bool frontCCW;
            FaceCulling culling;

            i32 depthBias;
            f32 depthBiasClamping;
            f32 slopeScaledDepthBias;

            bool renderWireframe;
            bool useDepthClipping;
            bool useAntialiasing;
            bool useMultisampling;
            bool useScissoring;
        };

        struct SamplerStateDescription {
        public:
            union {
                struct {
                    TextureAddressMode u;
                    TextureAddressMode v;
                    TextureAddressMode w;
                } addressMode;
                TextureAddressMode addressModes[3];
            };

            TextureFilterMode minificationFilter;
            TextureFilterMode magnificationFilter;
            TextureSampleMode samplingMode;
            bool shouldFilterMipmaps;
            ui32 maxAnisotropySamples;

            ComparisonMode comparison;
            f32v4 borderColor;

            f32 minLOD;
            f32 maxLOD;
            f32 lodBias;
        };

        struct QueryDescription {
        public:

        };

        struct PredicateDescription {
        public:

        };

        struct SyncFenceDescription {
        public:

        };

        struct Viewport {
        public:
            f32v2 min;
            f32v2 size;
            F32Range depth;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_ResourceDescriptions_h__
