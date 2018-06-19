//
// ThreadSync.h
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ThreadSync.h
 * @brief Handles synchronization barriers between threads via token consumption.
 */

#pragma once

#ifndef Vorb_ThreadSync_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ThreadSync_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <mutex>
#include <thread>

#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <condition_variable>

#include <Vorb/concurrentqueue.h>

/// Controls storage and retrieval of synchronization objects
template<typename T>
class ThreadSyncQueue {
public:
    /// Synchronization token
    class ThreadSyncObject {
        friend class ThreadSyncQueue<T>;
    public:
        /// Blocks current thread until consumption is performed
        void block() {
            std::unique_lock<std::mutex> lck(m_lock);
            if (m_hasCompleted) return;
            m_cond.wait(lck);
        }

        T data; ///< Internal data
    private:
        bool m_hasCompleted = false; ///< True if this object has been consumed and must no longer block
        std::mutex m_lock;
        std::condition_variable m_cond;
    };

    /// Add a token into the queue
    /// @param so: Object using for synchronization
    /// @param blockUntilFinished: True if current thread should block until object is consumed
    void add(ThreadSyncObject* so, bool blockUntilFinished = true) {
        std::unique_lock<std::mutex> lck(so->m_lock);
        so->m_hasCompleted = false;
        m_queue.enqueue(so);
        if(blockUntilFinished) so->m_cond.wait(lck);
    }

    /// Perform a bulk dequeue operation on the queue
    /// @tparam N: Size of objects array
    /// @param objects: Buffer to hold synchronization tokens
    /// @return Number of objects actually obtained
    template<size_t N>
    size_t getObjects(ThreadSyncObject* (&objects)[N]) {
        return m_queue.try_dequeue_bulk(objects, N);
    }
    
    /// Perform consumption and notification of a token
    /// @param so: The token to be updated
    void finish(ThreadSyncObject* so) {
        so->m_lock.lock();
        so->m_hasCompleted = true;
        so->m_cond.notify_all();
        so->m_lock.unlock();
    }
private:
    moodycamel::ConcurrentQueue<ThreadSyncObject*> m_queue; ///< Queue of objects to be consumed
};

#endif // !Vorb_ThreadSync_h__
