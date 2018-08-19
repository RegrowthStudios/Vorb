//
// ModelIO.h
// Vorb Engine
//
// Created by Cristian Zaloj on 18 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ModelIO.h
 * @brief Handles loading models into indexed-interleaved-vertex meshes.
 */

#pragma once

#ifndef Vorb_ModelIO_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ModelIO_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <unordered_map>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "MeshData.h"
#include "AnimationData.h"
#include "../utils.h"

namespace std {
    template<> struct hash<ui32v3>;
}

namespace vorb {
    namespace graphics {

        struct OBJMesh {
        public:
            std::vector<f32v3> positions;
            std::vector<f32v2> uvs;
            std::vector<f32v3> normals;

            std::unordered_map<ui32v3, ui32, std::hash<ui32v3>> vertices;
            std::vector<ui32v3> triangles;
        };

        class ModelIO {
        public:
            static ui32v2 loadOBJ(CALLER_DELETE const cString data, OUT OBJMesh& mesh);
            static CALLER_DELETE vg::MeshDataRaw loadRAW(CALLER_DELETE const void* data, OUT vg::VertexDeclaration& decl, OUT ui32& indexSize);

            static CALLER_DELETE vg::Skeleton loadAnim(CALLER_DELETE const void* data);
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_ModelIO_h__
