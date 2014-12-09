///
/// ThreadSync.h
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

#ifndef ThreadSync_h__
#define ThreadSync_h__

#include <condition_variable>

#include <concurrentqueue.h>

template<typename T>
class ThreadSyncQueue {
public:
    class ThreadSyncObject {
        friend class ThreadSyncQueue<T>;
    public:
        T data;

        void block() {
            std::unique_lock<std::mutex> lck(m_lock);
            if (m_hasCompleted) return;
            m_cond.wait(lck);
        }
    private:
        bool m_hasCompleted = false;
        std::mutex m_lock;
        std::condition_variable m_cond;
    };

    void add(ThreadSyncObject* so, bool blockUntilFinished = true) {
        std::unique_lock<std::mutex> lck(so->m_lock);
        so->m_hasCompleted = false;
        m_queue.enqueue(so);
        if(blockUntilFinished) so->m_cond.wait(lck);
    }

    template<size_t N>
    size_t getObjects(ThreadSyncObject* (&objects)[N]) {
        return m_queue.try_dequeue_bulk(objects, N);
    }

    void finish(ThreadSyncObject* so) {
        so->m_lock.lock();
        so->m_hasCompleted = true;
        so->m_cond.notify_all();
        so->m_lock.unlock();
    }
private:
    moodycamel::ConcurrentQueue<ThreadSyncObject*> m_queue;
};

#endif // ThreadSync_h__