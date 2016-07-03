//
// VoxelMesherCulled.h
// Vorb Engine
//
// Created by Cristian Zaloj on 27 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file VoxelMesherCulled.h
 * 
 * \brief Contains various algorithms for voxel meshing.
 */

#pragma once

#ifndef Vorb_VoxelMesherCulled_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VoxelMesherCulled_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "VoxCommon.h"
#include "VoxelMeshAlg.h"

namespace vorb {
    namespace voxel {
        namespace meshalg {
            /// Construct a voxel mesh, generating and culling desired face pairs
            /// \tparam T: Voxel data type
            /// \tparam API: Type of API object that handles culled meshing
            /// \param data: 3D array of voxel data accessed Y-Z-X
            /// \param size: Sizes of array (XYZ)
            /// \param apiCull: API object
            template<typename T, typename API>
            inline void createCulled(const T* data, const ui32v3& size, API* api) {
                static ui32v3 SWEEPS[3] = {
                    ui32v3(0, 2, 1),
                    ui32v3(1, 0, 2),
                    ui32v3(2, 0, 1)
                };
                static Axis AXES[3] = {
                    Axis::X,
                    Axis::Y,
                    Axis::Z
                };

                ui32v3 pos;
                size_t l1 = size.x;
                size_t l2 = l1 * size.z;

                for (size_t axis = 0; axis < 3; axis++) {
                    ui32& fAxis = pos[SWEEPS[axis].x];
                    ui32& uAxis = pos[SWEEPS[axis].y];
                    ui32& vAxis = pos[SWEEPS[axis].z];
                    ui32v3 sizes(size[SWEEPS[axis].x], size[SWEEPS[axis].y], size[SWEEPS[axis].z]);

                    VoxelQuad qNeg, qPos;
                    qPos.direction = toCardinal(AXES[axis], true);
                    qNeg.direction = toCardinal(AXES[axis], false);
                    qNeg.size = qPos.size = ui32v2(1, 1);

                    for (fAxis = 1; fAxis < sizes.x; fAxis++) {
                        for (uAxis = 1; uAxis < sizes.y - 1; uAxis++) {
                            for (vAxis = 1; vAxis < sizes.z - 1; vAxis++) {
                                fAxis--;
                                qPos.voxelPosition = pos;
                                qPos.startIndex = pos.y * l2 + pos.z * l1 + pos.x;
                                const T& v1 = data[qPos.startIndex];

                                fAxis++;
                                qNeg.voxelPosition = pos;
                                qNeg.startIndex = pos.y * l2 + pos.z * l1 + pos.x;
                                const T& v2 = data[qNeg.startIndex];

                                VoxelFaces f = api->occludes(v1, v2, AXES[axis]);
                                if (f.block1Face && fAxis != 1) api->result(qPos);
                                if (f.block2Face && fAxis != sizes.x - 1) api->result(qNeg);
                            }
                        }
                    }
                }
            }
        }
    }
}
namespace vvox = vorb::voxel;

#endif // !Vorb_VoxelMesherCulled_h__
