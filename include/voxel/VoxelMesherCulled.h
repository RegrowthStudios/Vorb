//
// VoxelMesherCulled.h
// Vorb Engine
//
// Created by Cristian Zaloj on 27 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file VoxelMesherCulled.h
 * @brief Contains various algorithms for voxel meshing.
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
            enum class VoxelOcclusion {
                DRAW_A = 0,
                DRAW_B = 1,
                OCCLUDE = 2
            };
            /// Construct a voxel mesh, generating and culling desired face pairs
            /// @tparam T: Voxel data type
            /// @tparam API: Type of API object that handles culled meshing
            /// @param data: 3D array of voxel data accessed Y-Z-X
            /// @param size: Sizes of array (XYZ)
            /// @param apiCull: API object
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
                size_t rowSize = size.x;
                size_t layerSize = rowSize * size.z;

                for (size_t axis = 0; axis < 3; axis++) {
                    ui32& fAxis = pos[SWEEPS[axis].x];
                    ui32& uAxis = pos[SWEEPS[axis].y];
                    ui32& vAxis = pos[SWEEPS[axis].z];
                    ui32v3 sizes(size[SWEEPS[axis].x], size[SWEEPS[axis].y], size[SWEEPS[axis].z]);

                    VoxelQuad qNeg, qPos;
                    qPos.direction = toCardinalPositive(AXES[axis]);
                    qNeg.direction = toCardinalNegative(AXES[axis]);
                    qNeg.size = qPos.size = ui32v2(1, 1);

                    for (fAxis = 1; fAxis < sizes.x - 1; fAxis++) {
                        for (uAxis = 0; uAxis < sizes.y; uAxis++) {
                            for (vAxis = 0; vAxis < sizes.z; vAxis++) {
                                fAxis--;
                                qPos.voxelPosition = pos;
                                qPos.startIndex = pos.y * layerSize + pos.z * rowSize + pos.x;
                                const T& v1 = data[qPos.startIndex];

                                fAxis++;
                                qNeg.voxelPosition = pos;
                                qNeg.startIndex = pos.y * layerSize + pos.z * rowSize + pos.x;
                                const T& v2 = data[qNeg.startIndex];

                                VoxelFaces f = api->occludes(v1, v2, AXES[axis]);
                                if (f.face1) api->result(qPos, v1);
                                if (f.face2) api->result(qNeg, v2);
                            }
                        }
                    }

                    //TODO(Ben) Outer Edge
                }
            }

        }
    }
}
namespace vvox = vorb::voxel;

#endif // !Vorb_VoxelMesherCulled_h__
