///
/// MeshGenerators.h
/// Vorb Engine
///
/// Created by Benjamin Arnold on 31 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Header file with definitions for mesh generation
///

#pragma once

#ifndef MeshGenerators_h__
#define MeshGenerators_h__

namespace vorb {
    namespace core {
        namespace mesh {
            /// Generates position and index buffer for an icosphere mesh
            /// @param lod: Number of subdivisions. 0 For lowest quality
            /// @param indices: Resulting index buffer
            /// @param positions: Resulting position buffer
            extern void generateIcosphereMesh(int lod, std::vector<ui32>& indices, std::vector<f32v3>& positions);
        }
    }
}
namespace vcore = vorb::core;
namespace vmesh = vcore::mesh;

#endif // MeshGenerators_h__