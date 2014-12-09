///
/// GLRPC.h
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

#ifndef GLRPC_h__
#define GLRPC_h__

#include "Events.hpp"
#include "gtypes.h"
#include "ThreadSync.h"

namespace vorb {
    namespace core {
		namespace graphics {
            struct GLRPCData {
                IDelegate<void*>* f = nullptr;
                void* userData = nullptr;
            };

            typedef ThreadSyncQueue<GLRPCData>::ThreadSyncObject GLRPC;
			
			#define GLRPC_REQUESTS_PER_PROCESS 8
			
			class GLRPCManager {
			public:
                void invoke(GLRPC* so, bool blockUntilFinished = true);
                size_t processRequests(size_t maxRequests = GLRPC_REQUESTS_PER_PROCESS);
			private:
                ThreadSyncQueue<GLRPCData> m_queue;
                std::queue<GLRPC*> m_unfinished;
			};
		}
    }
}
namespace vg = vorb::core::graphics;

#endif // GLRPC_h__