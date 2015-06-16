//
// InputDispatcher.h
// Vorb Engine
//
// Created by Cristian Zaloj on 10 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file InputDispatcher.h
 * @brief Dispatches OS and device inputs via events.
 */

#pragma once

#ifndef Vorb_InputDispatcher_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_InputDispatcher_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "KeyboardEventDispatcher.h"
#include "MouseInputDispatcher.h"
#include "WindowEventDispatcher.h"

namespace vorb {
    namespace ui {
        class GameWindow;
        namespace impl {
            VORB_INTERNAL class InputDispatcherEventCatcher;
        }

        /*! @brief Path drag'n'drop event wrapper.
         */
        struct DragDropEvent {
        public:
            const cString path; ///< Path of the drop (limited to 1).
        };

        /*! @brief Handles receiving and dispatching important events.
         * 
         * There exists an input dispatcher tied per window as well as a global
         * dispatcher that listens to application-wide events.
         */
        class InputDispatcher {
            friend class impl::InputDispatcherEventCatcher;
            friend class OSWindow;
        public:
            /*! @brief Begin pumping OS events through all the created input dispatchers.
             *
             * This will do nothing if init() has already been called successfully.
             *
             * @pre The UI implementation must have been initialized using vorb::init.
             */
            static void init();
            /*! @brief Stop pumping OS events through dispatchers.
             *
             * This will do nothing if init() has not yet been called.
             *
             * @pre The UI implementation must have been initialized using vorb::init.
             */
            static void dispose();

            static Event<const DragDropEvent&> onDragDrop; ///< Signaled when a path is dropped into the application
            static Event<> onQuit; ///< Signaled when application should quit

            MouseEventDispatcher mouse; ///< Dispatches mouse events.
            KeyboardEventDispatcher key; ///< Dispatches keyboard events.
            WindowEventDispatcher window; ///< Dispatches window events.
        private:
            volatile static bool m_isInit; ///< Keeps track of initialization status.

            OSWindow* m_window; ///< Window tracked by this dispatcher.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_InputDispatcher_h__
