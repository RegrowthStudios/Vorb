///
/// ThreadPool.h
/// Vorb Engine
///
/// Created by Benjamin Arnold on 13 Nov 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Provides a general threadpool implementation for
/// distributing work.
///

#pragma once

#ifndef ThreadPool_h__
#define ThreadPool_h__

#include <vector>
#include <thread>
#include <condition_variable>

#include "concurrentqueue.h"
#include "blockingconcurrentqueue.h"
#include "IThreadPoolTask.h"

class CAEngine;
class Chunk;
class ChunkMesher;
class FloraGenerator;
class VoxelLightEngine;
class LoadData;

enum class RenderTaskType;

namespace vorb {
    namespace core {
        template<typename T>
        class ThreadPool {
        public:
            ThreadPool() {};
            ~ThreadPool();

            /// Initializes the threadpool
            /// @param size: The number of worker threads
            void init(ui32 size);

            /// Frees all resources
            void destroy();

            /// Clears all unprocessed tasks from the task queue
            void clearTasks();

            /// Adds a task to the task queue
            /// @param task: The task to add
            void addTask(IThreadPoolTask<T>* task) {
                m_tasks.enqueue(task);
            }

            /// Add an array of tasks to the task queue
            /// @param tasks: The array of tasks to add
            /// @param size: The size of the array
            void addTasks(IThreadPoolTask<T>* tasks[], size_t size) {
                m_tasks.enqueue_bulk(tasks, size);
            }

            /// Gets a bulk of tasks from the finished tasks
            /// @param taskBuffer: Buffer to store the tasks
            /// @param maxSize: Max tasks to deque
            /// @return: The number of dequeued tasks
            size_t getFinishedTasks(IThreadPoolTask<T>** taskBuffer, size_t maxSize) {
                return m_finishedTasks.try_dequeue_bulk(taskBuffer, maxSize);
            }

            /// Getters
            i32 getNumWorkers() const { return m_workers.size(); }
            size_t getTasksSizeApprox() const { return m_tasks.size_approx(); }
            size_t getFinishedTasksSizeApprox() const { return m_finishedTasks.size_approx(); }
        private:
            VORB_NON_COPYABLE(ThreadPool);
            // Typedef for func ptr
            typedef void (ThreadPool<T>::*workerFunc)(T*);

            /// Class definition for worker thread
            class WorkerThread {
            public:
                /// Creates the thread
                /// @param func: The function the thread should execute
                WorkerThread(workerFunc func, ThreadPool<T>* threadPool) {
                    thread = new std::thread(func, threadPool, &data);
                }

                /// Blocks until the worker thread completes
                void join() {
                    thread->join();
                }

                std::thread* thread; ///< The thread handle
                T data; ///< Worker specific data
            };

            /// Thread function that processes tasks
            /// @param data: The worker specific data
            void workerThreadFunc(T* data);

            /// Lock free task queues
            moodycamel::BlockingConcurrentQueue<IThreadPoolTask<T>*> m_tasks; ///< Holds tasks to execute
            moodycamel::ConcurrentQueue<IThreadPoolTask<T>*> m_finishedTasks; ///< Holds finished tasks

            bool m_isInitialized = false; ///< true when the pool has been initialized
            std::vector<WorkerThread*> m_workers; ///< All the worker threads
        };

        template<typename T>
        class QuitThreadPoolTask : public IThreadPoolTask<T> {
            virtual void execute(T* workerData) override {
                workerData->stop = true;
            }
        };
    }
}
namespace vcore = vorb::core;

#include "ThreadPool.inl"

#endif // ThreadPool_h__