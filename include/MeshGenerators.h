//
// MeshGenerators.h
// Vorb Engine
//
// Created by Benjamin Arnold on 31 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file MeshGenerators.h
 * @brief Header file with definitions for mesh generation.
 */

#pragma once

#ifndef Vorb_MeshGenerators_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MeshGenerators_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <vector>

#include "types.h"
#endif // !VORB_USING_PCH

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

#endif // !Vorb_MeshGenerators_h__
