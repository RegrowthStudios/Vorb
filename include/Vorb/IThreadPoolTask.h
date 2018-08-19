//
// IThreadPoolTask.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 Nov 2014
// Copyright 2014 Regrowth Studios
// MIT License
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
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace core {

        template<typename T>
        class IThreadPoolTask {
        public:
            /// Constructor
            /// @param taskId: Optional unique identifier for task type.
            IThreadPoolTask(i32 taskId = -1) :
                m_taskId(taskId) {
                /* Empty */
            }

            virtual ~IThreadPoolTask() { /* Empty */ }

            /// Executes the task
            virtual void execute(T* workerData) = 0;

            /// Last thing that runs. Can delete or recycle the task here if you want.
            virtual void cleanup() {};
           
            /// Getters
            const i32& getTaskId() const { return m_taskId; }

            volatile bool isFinished = false;
        protected:
            i32 m_taskId;
        };
    }
}
namespace vcore = vorb::core;

#endif // !Vorb_IThreadPoolTask_h__
