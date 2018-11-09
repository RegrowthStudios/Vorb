//
// FocusController.h
// Vorb Engine
//
// Created by Benjamin Arnold on 20 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file FocusController.h
* @brief 
* Handles input focus.
*
*/

#pragma once

#ifndef Vorb_FocusController_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FocusController_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "../Events.hpp"
#include "MouseInputDispatcher.h"

namespace vorb {
    namespace ui {

        class FocusObject {
        public:
            virtual bool inBounds(f32 mouseX VORB_MAYBE_UNUSED, f32 mouseY VORB_MAYBE_UNUSED) { return true; }
            /* Events */
            Event<const MouseButtonEvent&> MouseDown; ///< Occurs when mouse button is pressed over container.
            Event<const MouseButtonEvent&> MouseUp; ///< Occurs when mouse button is released over container.
            Event<const MouseMotionEvent&> MouseMove; ///< Occurs when mouse pointer is moved over container.
            /* Members */
            f32 depth = 0.0f;
        };
        // TODO(Ben): Implement
        class FocusController {
        public:
            FocusController();
            ~FocusController();
            void init();
            void dispose();

            void addObject(FocusObject* obj);
            bool removeObject(const FocusObject* obj);

        protected:
            void onMouseDown(Sender s, const MouseButtonEvent& e);
            void onMouseUp(Sender s, const MouseButtonEvent& e);
            void onMouseMove(Sender s, const MouseMotionEvent& e);

            std::vector<FocusObject*> m_objects;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_FocusController_h__
