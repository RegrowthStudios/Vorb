#include "Vorb/stdafx.h"
#include "Vorb/voxel/VoxelTextureStitcher.h"
#include "Vorb/ecs/BitTable.hpp"

vvox::VoxelTextureStitcher::VoxelTextureStitcher(ui32 tilesPerRow /*= 16u*/) {
    m_tilesPerRow = tilesPerRow;
    m_tilesPerPage = tilesPerRow * tilesPerRow;
    // Leave space for a blank texture
    m_oldestFreeSlot = 1;
    // Always at least 1 page
    addPage();
}

vvox::VoxelTextureStitcher::~VoxelTextureStitcher() {
    for (auto page : m_pages) delete[] page;
}

ui32 vvox::VoxelTextureStitcher::mapSingle() {
    ui32 i;
    ui32 pageIndex;

    // Find the next free slot
    do {
        i = m_oldestFreeSlot % m_tilesPerPage;
        pageIndex = m_oldestFreeSlot / m_tilesPerPage;

        // If we need to allocate a new page
        if (pageIndex >= m_pages.size()) {
            addPage();
        }

        // Since we are mapping single textures, we know this is the oldest free
        m_oldestFreeSlot++;
    } while (m_pages[pageIndex][i] == true);

    //mark this slot as not free
    m_pages[pageIndex][i] = true;

    return pageIndex * m_tilesPerPage + i;
}

ui32 vvox::VoxelTextureStitcher::mapBox(ui32 width, ui32 height) {
    if (width > m_tilesPerRow || height > m_tilesPerRow) {
        return -1;
    }

    ui32 i;
    ui32 pageIndex;
    ui32 x, y;

    // Start the search at the oldest known free spot.
    ui32 searchIndex = m_oldestFreeSlot;
    bool fits;

    // Find the next free slot that is large enough
    while (true) {
        i = searchIndex % m_tilesPerPage;
        pageIndex = searchIndex / m_tilesPerPage;
        x = i % m_tilesPerRow;
        y = i / m_tilesPerRow;
        fits = true;

        // If we need to allocate a new atlas
        if (pageIndex >= m_pages.size()) {
            addPage();
        }

        //if it doesn't fit in Y direction, go to next page
        if (y + height > m_tilesPerRow) {
            searchIndex += m_tilesPerPage - i;
            continue;
        }
        //if it doesn't fit in X direction, go to next row
        if (x + width > m_tilesPerRow) {
            searchIndex += m_tilesPerRow - x;
            continue;
        }

        searchIndex++;

        //Search to see if all needed slots are free
        for (ui32 j = y; j < y + height; j++) {
            for (ui32 k = x; k < x + width; k++) {
                if (m_pages[pageIndex][j * m_tilesPerRow + k] == true) {
                    fits = false;
                    j = y + height; //force to fall out of loop
                    break;
                }
            }
        }

        if (fits) {
            //if we reach here, it will fit at this position
            break;
        }
    }

    //Set all slots to true
    for (ui32 j = y; j < y + height; j++) {
        for (ui32 k = x; k < x + width; k++) {
            m_pages[pageIndex][j * m_tilesPerRow + k] = true;
        }
    }

    return i + pageIndex * m_tilesPerPage;
}

ui32 vvox::VoxelTextureStitcher::mapContiguous(ui32 numTiles) {

    ui32 i;
    ui32 pageIndex;
    ui32 numContiguous = 0;
    // Start the search at the oldest known free spot.
    ui32 searchIndex = m_oldestFreeSlot;
    bool passedFreeSlot = false;

    // Find the next free slot that is large enough
    while (true) {
        i = searchIndex % m_tilesPerPage;
        pageIndex = searchIndex / m_tilesPerPage;

        // If we need to allocate a new atlas
        if (pageIndex >= m_pages.size()) {
            addPage();
        }

        searchIndex++;
        if (m_pages[pageIndex][i] == true) {
            // If we have any contiguous, then we left a free spot behind somewhere
            if (numContiguous) {
                passedFreeSlot = true;
            }
            numContiguous = 0;
        } else {
            numContiguous++;
        }

        // Stop searching if we have found a contiguous block that is large enough
        if (numContiguous == numTiles) {
            i = searchIndex % m_tilesPerPage;
            pageIndex = searchIndex / m_tilesPerPage;
            break;
        }
    }

    // Move the oldest known free slot forward if we havent passed a free spot
    if (passedFreeSlot == false) {
        m_oldestFreeSlot = i + pageIndex * m_tilesPerPage;
    }

    ui32 index = i + pageIndex * m_tilesPerPage - numTiles;

    // Mark slots as full
    for (searchIndex = index; searchIndex < index + numTiles; searchIndex++) {
        i = searchIndex % m_tilesPerPage;
        pageIndex = searchIndex / m_tilesPerPage;
        m_pages[pageIndex][i] = true;
    }

    return index;
}

void vvox::VoxelTextureStitcher::dispose() {
    for (auto page : m_pages) delete[] page;
    std::vector<bool*>().swap(m_pages);
}

void vvox::VoxelTextureStitcher::addPage() {
    m_pages.push_back(new bool[m_tilesPerPage] {});
}