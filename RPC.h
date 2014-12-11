///
/// RPC.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 8 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef RPC_h__
#define RPC_h__

#include <queue>

#include "Events.hpp"
#include "ThreadSync.h"

namespace vorb {
    namespace core {
        struct RPCData {
            IDelegate<void*>* f = nullptr;
            void* userData = nullptr;
        };

        typedef ThreadSyncQueue<RPCData>::ThreadSyncObject RPC;

#define RPC_REQUESTS_PER_PROCESS 8

        class RPCManager {
        public:
            void invoke(RPC* so, bool blockUntilFinished = true);
            size_t processRequests(size_t maxRequests = RPC_REQUESTS_PER_PROCESS);
        private:
            ThreadSyncQueue<RPCData> m_queue;
            std::queue<RPC*> m_unfinished;
        };
    }
}
namespace vcore = vorb::core;

#endif // RPC_h__