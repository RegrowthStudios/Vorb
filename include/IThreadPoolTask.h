//
// IThreadPoolTask.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file IThreadPoolTask.h
 * 
 * \brief This file provides a thread pool task interface.
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
        /*!
         * \class IThreadPoolTask
         *
         * \brief A task handled by the ThreadPool.
         *
         * \tparam T Data passed in for work.
         */
        template<typename T>
        class IThreadPoolTask {
        public:
            /*!
             * \fn IThreadPoolTask::IThreadPoolTask(i32 taskId = -1)
             *
             * \brief Initializes the task with its type.
             *
             * \param taskId (Optional) Unique identifier for task type.
             */
            IThreadPoolTask(i32 taskId = -1) :
                m_taskId(taskId) {
                /* Empty */
            }
            /*!
             * \fn virtual IThreadPoolTask::~IThreadPoolTask()
             *
             * \brief Destructor. Subclassed tasks probably want to perform a cleanup here.
             */
            virtual ~IThreadPoolTask() {
                /* Empty */
            }

            /*!
             * \fn virtual void IThreadPoolTask::execute(T* workerData) = 0;
             *
             * \brief Executes the task.
             *
             * \param [in,out] workerData Data used for task operation.
             */
            virtual void execute(T* workerData) = 0;
            /*!
             * \fn virtual void IThreadPoolTask::cleanup()
             *
             * \brief Last thing that runs.
             *  
             *  Can delete or recycle the task here if you want.
             */
            virtual void cleanup() {};

            /*!
             * \fn const i32& IThreadPoolTask::getTaskId() const
             *
             * \brief Get the unique type ID for this task.
             *
             * \return The task ID.
             */
            const i32& getTaskId() const {
                return m_taskId;
            }

            volatile bool isFinished = false; ///< Flag that is true if this task is finished.
        protected:
            i32 m_taskId; ///< The unique type identifier for this task.
        };
    }
}
namespace vcore = vorb::core;

#endif // !Vorb_IThreadPoolTask_h__
