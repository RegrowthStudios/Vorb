//
// ScriptValueSenders.h
// Vorb Engine
//
// Created by Cristian Zaloj on 18 Mar 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file ScriptValueSenders.h
 * 
 * \brief Defines a preset number of scripting conversion methods.
 */

#pragma once

#ifndef Vorb_ScriptValueSenders_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ScriptValueSenders_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH
#include "../types.h" // TODO(Ben): tmp
struct lua_State;

namespace vorb {
    namespace script {
        typedef lua_State* EnvironmentHandle; ///< The pointer to the scripting environment implementation
        /*! \brief Pre-declaration of the conversion API
         * 
         * \tparam T: Type of the value that can be converted into/from the scripting environment
         */
        template<typename T, typename = void> struct ScriptValueSender;

#define SCRIPT_SENDER_REF(TYPE) \
        template<> \
        struct ScriptValueSender<TYPE, void> { \
        public: \
            static TYPE defaultValue(); \
            static i32 getNumValues(); \
            static i32 push(EnvironmentHandle h, const TYPE& v); \
            static TYPE pop(EnvironmentHandle h); \
        }
#define SCRIPT_SENDER_VAL(TYPE) \
        template<> \
        struct ScriptValueSender<TYPE, void> { \
        public: \
            static TYPE defaultValue(); \
            static i32 getNumValues(); \
            static i32 push(EnvironmentHandle h, TYPE v); \
            static TYPE pop(EnvironmentHandle h); \
                        }
#define SCRIPT_SENDER_VEC(TYPE) \
        SCRIPT_SENDER_VAL(TYPE); \
        SCRIPT_SENDER_REF(TYPE##v2); \
        SCRIPT_SENDER_REF(TYPE##v3); \
        SCRIPT_SENDER_REF(TYPE##v4)

        SCRIPT_SENDER_VEC(i8);
        SCRIPT_SENDER_VEC(i16);
        SCRIPT_SENDER_VEC(i32);
        SCRIPT_SENDER_VEC(i64);
        SCRIPT_SENDER_VEC(ui8);
        SCRIPT_SENDER_VEC(ui16);
        SCRIPT_SENDER_VEC(ui32);
        SCRIPT_SENDER_VEC(ui64);
        SCRIPT_SENDER_VEC(f32);
        SCRIPT_SENDER_VEC(f64);
        SCRIPT_SENDER_VAL(bool);
        SCRIPT_SENDER_REF(nString);
        SCRIPT_SENDER_VAL(const cString);
        SCRIPT_SENDER_VAL(void*);
        SCRIPT_SENDER_REF(color4);

        template<typename T>
        struct ScriptValueSender<T, typename std::enable_if<std::is_enum<T>::value>::type> {
        public:
            typedef typename std::underlying_type<T>::type EnumType;

            static T defaultValue() {
                return static_cast<T>(0);
            }
            static i32 getNumValues() {
                return ScriptValueSender<EnumType>::getNumValues();
            }
            static i32 push(EnvironmentHandle h, T v) {
                return ScriptValueSender<EnumType>::push(h, static_cast<EnumType>(v));
            }
            static T pop(EnvironmentHandle h) {
                return static_cast<T>(ScriptValueSender<EnumType>::pop(h));
            }
        };

        template<typename T>
        struct ScriptValueSender<T*, typename std::enable_if<std::is_pointer<T*>::value>::type> {
        public:
            static T* defaultValue() {
                return (T*)nullptr;
            }
            static i32 getNumValues() {
                return ScriptValueSender<void*>::getNumValues();
            }
            static i32 push(EnvironmentHandle h, T* v) {
                return ScriptValueSender<void*>::push(h, v);
            }
            static T* pop(EnvironmentHandle h) {
                return (T*)ScriptValueSender<void*>::pop(h);
            }
        };

        template<typename T>
        struct ScriptValueSender<T&, typename std::enable_if<std::is_reference<T&>::value>::type> {
        public:
            static T& defaultValue() {
                T* t = (T*)nullptr;
                return *t;
            }
            static i32 getNumValues() {
                return ScriptValueSender<void*>::getNumValues();
            }
            static i32 push(EnvironmentHandle h, T& v) {
                return ScriptValueSender<void*>::push(h, &v);
            }
            static T& pop(EnvironmentHandle h) {
                return *((T*)ScriptValueSender<void*>::pop(h));
            }
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_ScriptValueSenders_h__
