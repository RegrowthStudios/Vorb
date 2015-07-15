//
// VoxCommon.h
// Vorb Engine
//
// Created by Cristian Zaloj on 27 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file VoxCommon.h
 * @brief Common code in voxel environments.
 */

#pragma once

#ifndef Vorb_VoxCommon_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VoxCommon_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace voxel {
        /// 3D axes
        enum class Axis {
            X = 0x00, ///< X-axis (0)
            Y = 0x01, ///< Y-axis (1)
            Z = 0x02 ///< Z-axis (2)
        };
        ENUM_CLASS_OPS_INL(Axis, ui8)

        /// A direction on an axis
        enum class Cardinal {
            NEGATIVE = 0x00, ///< Negative direction (lowest bit = 0)
            POSITIVE = 0x01, ///< Positive direction (lowest bit = 1)
            X = 0x00, ///< X-axis (Axis::X << 1)
            Y = 0x02, ///< Y-axis (Axis::Y << 1)
            Z = 0x04, ///< Z-axis (Axis::Z << 1)
            X_NEG = X | NEGATIVE, ///< Negative direction on x-axis
            X_POS = X | POSITIVE, ///< Positive direction on x-axis
            Y_NEG = Y | NEGATIVE, ///< Negative direction on y-axis
            Y_POS = Y | POSITIVE, ///< Positive direction on y-axis
            Z_NEG = Z | NEGATIVE, ///< Negative direction on z-axis
            Z_POS = Z | POSITIVE ///< Positive direction on z-axis
        };
        ENUM_CLASS_OPS_INL(Cardinal, ui8)


        /// Create a cardinal direction
        /// @param a: Axis
        /// @param positive: True if facing in positive direction
        /// @return Cardinal direction
        Cardinal toCardinal(const Axis& a, const bool& positive);
        /// Extract axis information from cardinal direction
        /// @param c: Cardinal direction
        /// @return Axis of the cardinal direction
        Axis toAxis(const Cardinal& c);
    }
}
namespace vvox = vorb::voxel;

#endif // !Vorb_VoxCommon_h__
