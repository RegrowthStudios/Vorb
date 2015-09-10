//
// IThreadPoolTask.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IThreadPoolTask.h
 * @brief This file provides a thread pool task interface.
 */

#pragma once

#ifndef Vorb_IThreadPoolTask_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IThreadPoolTask_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

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

            /// Last thing that runs. Can delete or recycle the task here if you want.
            virtual void cleanup() {};

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
namespace vcore = vorb::core;

#endif // !Vorb_IThreadPoolTask_h__
