//
// VoxelTextureStitcher.h
// Vorb Engine
//
// Created by Benjamin Arnold on 9 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file VoxelTextureStitcher.h
* @brief Maps voxel tile textures to an atlas.
*
*
*/

#pragma once

#ifndef Vorb_VoxelTextureStitcher_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VoxelTextureStitcher_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

/// Stores Information About An Atlas Page For Construction Purposes
class BlockAtlasPage;

// TODO(Ben): Bitfield for smaller mem usage
/*! @brief Maps textures into a texture atlas
*/
namespace vorb {
    namespace voxel {
        class VoxelTextureStitcher {
        public:
            /*! @brief Constructor
            *
            * Allocates a single page and maps a null texture index
            */
            VoxelTextureStitcher(ui32 tilesPerRow = 16u);
            /*! @brief Necessary destructor to free allocated pages
            */
            virtual ~VoxelTextureStitcher();

            /// Maps a single block texture to the atlases
            /// @return The index of the texture start into the atlas array.
            ui32 mapSingle();
            /// Maps a large box texture to the atlases
            /// @param width: The width of the box
            /// @param height: The height of the box
            /// @return The index of the texture start into the atlas array.
            ui32 mapBox(ui32 width, ui32 height);
            /// Maps a contiguous array of single textures to the atlases
            /// @param numTiles: The number of tiles to map
            /// @return The index of the texture start into the atlas array.
            ui32 mapContiguous(ui32 numTiles);

            /*! @brief Retrieve the number of pages currently allocated by the mapper
            *
            * @return Number of allocated pages
            */
            size_t getNumPages() const {
                return m_pages.size();
            }

            void dispose();
        private:
            VORB_NON_COPYABLE(VoxelTextureStitcher);
            void addPage();

            std::vector<bool*> m_pages; ///< list of pages. A page is just an array of bools.
            ui32 m_oldestFreeSlot; ///< The left-most free slot in the atlas array
            ui32 m_tilesPerPage;
            ui32 m_tilesPerRow;
        };
    }
}
namespace vvox = vorb::voxel;

#endif // !Vorb_VoxelTextureStitcher_h__
