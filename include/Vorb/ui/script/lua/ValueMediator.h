//
// ValueMediator.cpp
// Vorb Engine
//
// Created by Matthew Marshall on 17 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file ValueMediator.cpp
 * \brief UI extension of Lua ValueMediator
 */

#pragma once

#ifndef Vorb_UI_Lua_ValueMediator_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UI_Lua_ValueMediator_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/script/lua/ValueMediator.h"

DECL_VUI(struct Clipping; struct Dock;    struct WidgetFlags; struct Length;
         struct Length2;  struct Length4; struct MouseEvent;  struct MouseButtonEvent;
         struct MouseMotionEvent;  struct MouseWheelEvent; struct WindowFileEvent;
         struct WindowResizeEvent)

namespace vorb {
    namespace script {
        namespace lua {
#define VALUE_MEDIATOR_VAL(TYPE)                                                    \
            template<>                                                              \
            struct ValueMediator<TYPE, void> {                                      \
            public:                                                                 \
                static TYPE defaultValue();                                         \
                static i32 getValueCount();                                         \
                static i32 push(Handle state, TYPE value);                          \
                static TYPE pop(Handle state);                                      \
                static bool tryPop(Handle state, OUT TYPE& value);                  \
                static TYPE retrieve(Handle state, ui32 index);                     \
                static bool tryRetrieve(Handle state, ui32 index, OUT TYPE& value); \
            }
#define VALUE_MEDIATOR_REF(TYPE)                                                    \
            template<>                                                              \
            struct ValueMediator<TYPE, void> {                                      \
            public:                                                                 \
                static TYPE defaultValue();                                         \
                static i32 getValueCount();                                         \
                static i32 push(Handle state, const TYPE& value);                   \
                static TYPE pop(Handle state);                                      \
                static bool tryPop(Handle state, OUT TYPE& value);                  \
                static TYPE retrieve(Handle state, ui32 index);                     \
                static bool tryRetrieve(Handle state, ui32 index, OUT TYPE& value); \
            }

            VALUE_MEDIATOR_VAL(vui::Clipping);
            VALUE_MEDIATOR_VAL(vui::Dock);
            VALUE_MEDIATOR_VAL(vui::WidgetFlags);
            VALUE_MEDIATOR_VAL(vui::Length);
            VALUE_MEDIATOR_REF(vui::Length2);
            VALUE_MEDIATOR_REF(vui::Length4);

            VALUE_MEDIATOR_REF(vui::MouseEvent);
            VALUE_MEDIATOR_REF(vui::MouseButtonEvent);
            VALUE_MEDIATOR_REF(vui::MouseMotionEvent);
            VALUE_MEDIATOR_REF(vui::MouseWheelEvent);

            VALUE_MEDIATOR_REF(vui::WindowResizeEvent);
            VALUE_MEDIATOR_REF(vui::WindowFileEvent);
#undef VALUE_MEDIATOR_VAL
        }
    }
}
namespace vscript = vorb::script;

#endif // VORB_USING_SCRIPT

#endif // !Vorb_UI_Lua_ValueMediator_h__
