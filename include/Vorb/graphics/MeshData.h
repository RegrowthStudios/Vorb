//
// MeshData.h
// Vorb Engine
//
// Created by Cristian Zaloj on 18 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file MeshData.h
 * @brief 
 */

#pragma once

#ifndef Vorb_MeshData_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MeshData_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "GLEnums.h"

namespace vorb {
    namespace graphics {
        enum class VertexAttributeUsage : ui8 {
            Position = 0,
            TextureCoordinate = 1,
            Normal = 2,
            Color = 3,
            Binormal = 4,
            Tangent = 5,
            BoneWeights = 6,
            BoneIndices = 7,
            Custom = 8
        };
        ENUM_CLASS_OPS_INL(VertexAttributeUsage, ui8);
        
        enum class VertexElementFlags : ui8 {
            FLOAT = 0x01,
            UNSIGNED = 0x02,
            NORMALIZED = 0x04
        };
        ENUM_CLASS_OPS_INL(VertexElementFlags, ui8);

        inline vg::VertexAttribPointerType getComponentType(VertexElementFlags f, size_t size) {
            if ((f & VertexElementFlags::FLOAT) == VertexElementFlags::FLOAT) {
                switch (size) {
                case 2: return vg::VertexAttribPointerType::HALF_FLOAT;
                case 4: return vg::VertexAttribPointerType::FLOAT;
                case 8: return vg::VertexAttribPointerType::DOUBLE;
                }
            } else if ((f & VertexElementFlags::UNSIGNED) == VertexElementFlags::UNSIGNED) {
                switch (size) {
                case 1: return vg::VertexAttribPointerType::UNSIGNED_BYTE;
                case 2: return vg::VertexAttribPointerType::UNSIGNED_SHORT;
                case 4: return vg::VertexAttribPointerType::UNSIGNED_INT;
                }
            } else {
                switch (size) {
                case 1: return vg::VertexAttribPointerType::BYTE;
                case 2: return vg::VertexAttribPointerType::SHORT;
                case 4: return vg::VertexAttribPointerType::INT;
                }
            }
            return vg::VertexAttribPointerType::NONE;
        }

        union VertexAttributeIndexed {
            struct {
                VertexAttributeUsage type : 4;
                ui8 index : 4;
            };
            ui8 value;
        };
        static_assert(sizeof(VertexAttributeIndexed) == 1, "Bad struct size");

        struct VertexElement {
        public:
            ui16 offset;
            ui8 componentSize;
            ui8 componentCount;
            VertexAttributeIndexed usage;
            VertexElementFlags flags;
        };
        static_assert(sizeof(VertexElement) == 6, "Bad struct size");


        typedef Array<VertexElement> VertexDeclaration;

        template<typename V, typename I>
        struct MeshData {
        public:
            V* vertices;
            size_t vertexCount;
            I* indices;
            size_t indexCount;
        };


        struct MeshDataRaw {
        public:
            template<typename V, typename I>
            MeshData<V, I> as() const {
                MeshData<V, I> m;
                m.vertices = vertices;
                m.vertexCount = vertexCount;
                m.indices = indices;
                m.indexCount = indexCount;
                return m;
            }

            void* vertices;
            ui32 vertexCount;
            void* indices;
            ui32 indexCount;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_MeshData_h__
