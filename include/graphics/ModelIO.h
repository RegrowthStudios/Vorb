///
/// ModelIO.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 18 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Handles loading models into indexed-interleaved-vertex meshes
///

#pragma once

#ifndef ModelIO_h__
#define ModelIO_h__

#include "MeshData.h"
//#include "utils.h"

namespace std {
    template<> struct hash<ui32v3>;
}

namespace vorb {
    namespace io {

        struct OBJMesh {
        public:
            std::vector<f32v3> positions;
            std::vector<f32v2> uvs;
            std::vector<f32v3> normals;

            std::unordered_map<ui32v3, ui32, std::_Bitwise_hash<ui32v3>> vertices;
            std::vector<ui32v3> triangles;
        };

        class ModelIO {
        public:
            static ui32v2 loadOBJ(CALLER_DELETE const cString data, OUT OBJMesh& mesh);
            static CALLER_DELETE vg::MeshDataRaw loadRAW(CALLER_DELETE const void* data, OUT vg::VertexDeclaration& decl, OUT size_t& indexSize);
        };
    }
}
namespace vio = vorb::io;

#endif // ModelIO_h__
