///
/// IThreadPoolTask.h
/// Seed of Andromeda
///
/// Created by Benjamin Arnold on 10 Nov 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// This file provides a thread pool task interface
///

#pragma once

#ifndef ThreadPoolTask_h__
#define ThreadPoolTask_h__

namespace vorb {
    namespace core {

        template<typename T>
        class IThreadPoolTask {
        public:
            /// Constructor
            /// @param taskId: Optional unique identifier for task type.
            IThreadPoolTask(bool shouldAddToFinishedTasks = false, i32 taskId = -1) :
                m_shouldAddToFinishedTasks(shouldAddToFinishedTasks),
                m_taskId(taskId) {
                /* Empty */
            }

            virtual ~IThreadPoolTask() { /* Empty */ }

            /// Executes the task
            virtual void execute(T* workerData) = 0;

            /// Checks if this should be stored in a finished tasks queue
            virtual const bool& shouldAddToFinishedTasks() const { return m_shouldAddToFinishedTasks; }

            /// Setters
            void setIsFinished(bool isFinished) { m_isFinished = isFinished; }
            void setShouldAddToFinishedtasks(bool shouldAdd) { m_shouldAddToFinishedTasks = shouldAdd; }

            /// Getters
            const i32& getTaskId() const { return m_taskId; }
            const volatile bool& getIsFinished() const { return m_isFinished; }

        protected:
            i32 m_taskId;
            volatile bool m_isFinished = false;
            bool m_shouldAddToFinishedTasks; ///< SHould it be stored in a finished tasks queue
        };
    }
}

// Namespace alias
namespace vcore = vorb::core;

#endif // ThreadPoolTask_h__