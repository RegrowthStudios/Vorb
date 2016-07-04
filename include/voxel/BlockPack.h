//
// BlockPack.h
// Vorb Engine
//
// Created by Benjamin Arnold on 3 Jul 2016
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file BlockPack.h
* @brief Flyweight container for blocks.
*
*
*/

#pragma once

#ifndef Vorb_BlockPack_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_BlockPack_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "../Events.hpp"

namespace vorb {
namespace voxel {

    typedef nString BlockIdentifier; ///< Unique identifier key for blocks

    /*! @brief All custom Block types should inherit from this.
     */
    template <typename BLOCKID>
    class Block {
    public:
        virtual ~Block() {};
        BlockIdentifier SID;
        BLOCKID ID;
    };

    /*! @brief A pack of blocks.
     *  You should define a BLOCK type that inherits from vvox::Block.
     *  BLOCKINDEX should be an unsigned integer type, that is larger than the maximum number of unique blocks.
     */
    template <typename BLOCK, typename BLOCKINDEX, typename BLOCKID>
    class BlockPack {
    public:
        BlockPack() {}

        /*! @brief Add a block to the pack, and overwrite a block of the same BlockIdentifier
         * Will invalidate existing BLOCK* pointers. Store BlockIDs instead.
         * @return the BLOCKID of the new block, or of the existing block if this block is already added.
         */
        BLOCKID append(BLOCK& block) {
            const BLOCK* curBlock;
            BLOCKID rv;
            if (curBlock = hasBlock(block.SID)) {
                rv = curBlock->ID;
                block.ID = rv;
                // Overwrite block
                *const_cast<BLOCK*>(curBlock) = block;
            } else {
                rv = m_blockList.size();
                block.ID = rv;
                // Add a new block
                m_blockList.push_back(block);
                // Set the correct index
                m_blockMap[block.SID] = rv;
            }
            onBlockAddition(block.ID);
            return rv;
        }

        void reserveID(const BlockIdentifier& sid, const BLOCKID& id) {
            if (id >= m_blockList.size()) m_blockList.resize(id + 1);
            m_blockMap[sid] = id;
            m_blockList[id].ID = id;
        }

        
        const BLOCK* hasBlock(const BLOCKID& id) const {
            if (id >= m_blockList.size()) {
                return nullptr;
            } else {
                return &m_blockList[id];
            }
        }
        const BLOCK* hasBlock(const BlockIdentifier& sid) const {
            auto v = m_blockMap.find(sid);
            if (v == m_blockMap.end()) {
                return nullptr;
            } else {
                return &m_blockList[v->second];
            }
        }

        size_t size() const {
            return m_blockList.size();
        }

        /************************************************************************/
        /* BLOCK accessors                                                      */
        /************************************************************************/
        BLOCK& operator[](const size_t& index) {
            return m_blockList[index];
        }
        const BLOCK& operator[](const size_t& index) const {
            return m_blockList[index];
        }
        BLOCK& operator[](const BlockIdentifier& sid) {
            return m_blockList[m_blockMap.at(sid)];
        }
        const BLOCK& operator[](const BlockIdentifier& sid) const {
            return m_blockList[m_blockMap.at(sid)];
        }
        const BLOCKINDEX& getBlockIndex(const BlockIdentifier& sid) const {
            return m_blockMap.at(sid);
        }

        const std::unordered_map<BlockIdentifier, BLOCKINDEX>& getBlockMap() const { return m_blockMap; }
        const std::vector<BLOCK>& getBlockList() const { return m_blockList; }

        Event<BLOCKINDEX> onBlockAddition; ///< Signaled when a block is loaded
    private:
        std::unordered_map<BlockIdentifier, BLOCKINDEX> m_blockMap; ///< Blocks indices organized by identifiers
        std::vector<BLOCK> m_blockList; ///< BLOCK data list
    };

}
}
namespace vvox = vorb::voxel;

#endif // !Vorb_BlockPack_h__

