//
// IDGenerator.h
// Vorb Engine
//
// Created by Cristian Zaloj on 30 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IDGenerator.h
 * @brief Unique ID generator and recycler.
 */

#pragma once

#ifndef Vorb_IDGenerator_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IDGenerator_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH
#include <queue>

namespace vorb {
    namespace core {
#define ID_GENERATOR_NULL_ID 0

        /// Generates and recycles unique IDs of a certain type
        /// @tparam T: must support operator size_t() and operator++()
        template<typename T>
        class IDGenerator {
        public:
            /// Grabs an unique ID from either generation of recycling
            /// @param wasNew: Additional return value to determine if a new ID was created
            /// @return An unique ID
            T generate(OPT bool* wasNew = nullptr) {
                T v;
                if (m_recycled.size() > 0) {
                    v = m_recycled.front();
                    m_recycled.pop();
                    if (wasNew) *wasNew = false;
                } else {
                    m_currentID++;
                    v = m_currentID;
                    if (wasNew) *wasNew = true;
                }
                return v;
            }
            /// Returns an ID to a recycle queue
            /// @pre: ID value is not in the recycle queue already
            /// @param v: ID to recycle
            void recycle(const T& v) {
                m_recycled.push(v);
            }

            /// Reset this generator to a fresh state
            void reset() {
                m_currentID = ID_GENERATOR_NULL_ID;
                std::queue<T>().swap(m_recycled);
            }

            /// @return Number of active IDs
            size_t getActiveCount() const {
                return static_cast<size_t>(m_currentID) - m_recycled.size();
            }
        private:
            T m_currentID = ID_GENERATOR_NULL_ID; ///< Auto-incremented ID
            std::queue<T> m_recycled; ///< List of recycled IDs
        };
    }
}
namespace vcore = vorb::core;

#endif // !Vorb_IDGenerator_h__
