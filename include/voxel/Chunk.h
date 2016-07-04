//
// Chunk.h
// Vorb Engine
//
// Created by Benjamin Arnold on 3 Jul 2016
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Chunk.h
* @brief Defines a voxel chunk for the vorb engine.
*
*
*/

#pragma once

#ifndef Vorb_Chunk_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Chunk_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "SmartVoxelContainer.hpp"

namespace vorb {
namespace voxel {

    // Type for block data and tertiary data
    template<typename TB, typename TT, ui32 WIDTH>
    class Chunk {
    public:

        const ui32 width = WIDTH;
        const ui32 size = WIDTH * WIDTH * WIDTH;

        SmartVoxelContainer<TB, WIDTH * WIDTH * WIDTH> blockData;
        SmartVoxelContainer<TB, WIDTH * WIDTH * WIDTH> tertiaryData;

    };


}
}
namespace vvox = vorb::voxel;

#endif // !Vorb_Chunk_h__
