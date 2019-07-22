//
// VoxelMeshAlg.h
// Vorb Engine
//
// Created by Cristian Zaloj on 27 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file VoxelMeshAlg.h
 * @brief Defines general API for voxel mesh algorithms.
 */

#pragma once

#ifndef Vorb_VoxelMeshAlg_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VoxelMeshAlg_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "VoxCommon.h"

namespace vorb {
    namespace voxel {
        namespace meshalg {
            /// Two flags specifying which faces are visible
            struct VoxelFaces {
            public:
                bool block1Face : 1; ///< First block face's visibility
                bool block2Face : 1; ///< Second block face's visibility
            };

            /// A quad created in a voxel mesh surface
            struct VoxelQuad {
            public:
                ui32v3 voxelPosition; ///< Position of voxel in data array
                ui32 startIndex; ///< Voxel index of starting point
                ui32v2 size; ///< Size of the quad
                Cardinal direction; ///< Direction the quad is facing
            };

            /// Create an index list for quads
            /// @tparam T: Index type/size
            /// @param quads: Number of quads for which indices must be specified
            /// @param startIndex: The index of the first vertex
            /// @return Array of quad vertex indices
            template<typename T>
            CALLER_DELETE T* generateQuadIndices(const ui32& quads, T startIndex = 0) {
                size_t ic = quads * 6;
                T* inds = new T[ic];
                T vi = startIndex;
                for (size_t ii = 0; ii < ic;) {
                    inds[ii++] = vi;
                    inds[ii++] = vi + 2;
                    inds[ii++] = vi + 1;
                    inds[ii++] = vi + 1;
                    inds[ii++] = vi + 2;
                    inds[ii++] = vi + 3;
                    vi += 4;
                }
                return inds;
            }
        }
    }
}
namespace vvox = vorb::voxel;

#endif // !Vorb_VoxelMeshAlg_h__
