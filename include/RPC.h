//
// RPC.h
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file RPC.h
 * 
 * \brief Handles cross-thread function invocation and synchronization.
 */

#pragma once

#ifndef Vorb_RPC_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_RPC_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <queue>

#include "Events.hpp"
#include "ThreadSync.h"

namespace vorb {
    namespace core {
        typedef Delegate<Sender, void*> RPCFunction; ///< Supported RPC function prototype

        struct RPCData {
            RPCFunction* f = nullptr; ///< Function to invoke on separate thread
            void* userData = nullptr; ///< Additional data passed on function invocation
        };

        typedef ThreadSyncQueue<RPCData>::ThreadSyncObject RPC; ///< RPC call handle

#define RPC_REQUESTS_PER_PROCESS 8

        /// Manages cross-thread function calls
        class RPCManager {
        public:
            /// Reports to target thread that a cross-thread call should be invoked upon it
            /// \param so: RPC handle, can be used later for synchronization
            /// \param blockUntilFinished: True if current thread should be blocked until RPC finishes
            void invoke(RPC* so, bool blockUntilFinished = true);
            /// Invokes cross-thread calls, method should be called on the target thread
            /// \param maxRequests: Maximum number of requests to process (clamped to RPC_REQUESTS_PER_PROCESS)
            /// \return Number of requests that were processed
            size_t processRequests(size_t maxRequests = RPC_REQUESTS_PER_PROCESS);
        private:
            ThreadSyncQueue<RPCData> m_queue; ///< Queue of idle requests
            std::queue<RPC*> m_unfinished; ///< Queue of taken, but unfinished requests
        };
    }
}
namespace vcore = vorb::core;

#endif // !Vorb_RPC_h__
