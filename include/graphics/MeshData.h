///
/// MeshData.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 18 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
///
///

#pragma once

#ifndef MeshData_h__
#define MeshData_h__

namespace vorb {
    namespace graphics {
        enum class VertexAttributeUsage {
            Position,
            TextureCoordinate,
            Normal,
            Color,
            Binormal,
            Tangent,
            Custom
        };

        struct VertexElement {
        public:
            VertexAttributeUsage usage : 4;
            ui32 usageIndex : 4;
            size_t offset;
            ui16 componentSize;
            ui16 componentCount;
            bool isFloat : 1;
            bool isUnsigned : 1;
        };



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
            size_t vertexCount;
            void* indices;
            size_t indexCount;
        };
    }
}
namespace vg = vorb::graphics;

#endif // MeshData_h__