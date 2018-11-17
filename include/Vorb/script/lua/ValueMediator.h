//
// lua/ValueMediator.h
// Vorb Engine
//
// Created by Matthew Marshall on 8 Nov 2018
// Adapted (read, basically entirely lifted) from original ScriptValueSenders by Cristian Zaloj.
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file ValueMediator.h
 * \brief Value mediator between Lua script env and C++.
 */

#pragma once

#ifndef Vorb_Lua_ValueMediator_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Lua_ValueMediator_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include <lua.hpp>

#include "Vorb/types.h"

namespace vorb {
    namespace script {
        namespace lua {
            using Handle = lua_State*;

            // Forward declare the ValueMediator struct.
            // This struct is used to facilitate simpler templated pushing and popping of common types onto the lua state stack.
            template<typename Type, typename = void> struct ValueMediator;

            // Define a couple of macros to declare the various ValueMediator specialisations.
            // In principle the type of the value to be pushed or popped could take up multiple values on the stack (e.g. a 2D vector would be 2 number values),
            // so we return an integer from push indicating the number of values pushed onto the stack.
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
#define VALUE_MEDIATOR_VEC(TYPE)      \
        VALUE_MEDIATOR_VAL(TYPE);     \
        VALUE_MEDIATOR_REF(TYPE##v2); \
        VALUE_MEDIATOR_REF(TYPE##v3); \
        VALUE_MEDIATOR_REF(TYPE##v4)

            VALUE_MEDIATOR_VEC(i8);
            VALUE_MEDIATOR_VEC(i16);
            VALUE_MEDIATOR_VEC(i32);
            VALUE_MEDIATOR_VEC(i64);
            VALUE_MEDIATOR_VEC(ui8);
            VALUE_MEDIATOR_VEC(ui16);
            VALUE_MEDIATOR_VEC(ui32);
            VALUE_MEDIATOR_VEC(ui64);
            VALUE_MEDIATOR_VEC(f32);
            VALUE_MEDIATOR_VEC(f64);
            VALUE_MEDIATOR_VAL(bool);
            VALUE_MEDIATOR_REF(nString);
            VALUE_MEDIATOR_VAL(const cString);
            VALUE_MEDIATOR_VAL(void*);
            VALUE_MEDIATOR_REF(color4);

#undef VALUE_MEDIATOR_VEC
#undef VALUE_MEDIATOR_REF
#undef VALUE_MEDIATOR_VAL

#if defined(VORB_OS_WINDOWS)
            template<typename Type>
            i32 getTotalValueCount() {
                return ValueMediator<Type>::getValueCount();
            }
            template<typename Type1, typename Type2, typename ...Types>
            i32 getTotalValueCount() {
                return getTotalValueCount<Type2, Types...>()
                    + getTotalValueCount<Type1>();
            }
#else
            template<typename ...Types>
            i32 getTotalValueCount() {
                i32 temp = 0;
                for (i32 size : { 0, ValueMediator<Types>::getValueCount()... }) {
                    temp += size;
                }
                return temp;
            }
#endif

            namespace impl {
                /*!
                 * \brief rpopValues provides a recursive method to pop all expected values from the Lua stack.
                 *
                 * These three functions provide a recursive method to pop all expected values from
                 * the lua state stack. The list of value types provided is the reverse order to the 
                 * order of the values on the stack. E.g. for parameters <int, double, nString>
                 * we would expect the top value on the stack to be a C string, the second to top a double
                 * and the third to be an integer.
                 *
                 * \param state The Lua stack.
                 * \param tPtr A pointer to the tuple we wish to populate.
                 */
                template<typename Type>
                void rpopValues(Handle state, OUT std::tuple<Type>* tPtr) {
                    // Pop one value of type Type from stack and put in tuple.
                    std::get<0>(*tPtr) = ValueMediator<Type>::pop(state);
                }
                template<typename Type>
                void rpopValues(Handle state, OUT std::tuple<Type&>* tPtr) {
                    // Copy the pointer to the value into the tuple.
                    void* value = ValueMediator<void*>::pop(state);
                    std::memcpy(tPtr, &value, sizeof(void*));
                }
                template<typename Type1, typename Type2, typename ...Types>
                void rpopValues(Handle state, OUT std::tuple<Type1, Type2, Types...>* tPtr) {
                    // Pop all but the first value - we want this recursive approach to pop in reverse order of
                    // types provided.
                    std::tuple<Type2, Types...> tToPop{ ValueMediator<Type2>::defaultValue(), ValueMediator<Types>::defaultValue()... };
                    rpopValues<Type2, Types...>(state, &tToPop);

                    // Pop the first value.
                    std::tuple<Type1> tPopped{ ValueMediator<Type1>::defaultValue() };
                    rpopValues<Type1>(state, &tPopped);

                    // Concatenate tuples and swap with the pointer provided.
                    auto catRes = std::tuple_cat(tPopped, tToPop);
                    std::swap(*tPtr, catRes);
                }

                /*!
                 * \brief rpopValues provides a recursive method to safely pop all expected values from the Lua stack.
                 *
                 * These three functions provide a recursive method to safely pop all expected values from
                 * the lua state stack. The list of value types provided is the reverse order to the 
                 * order of the values on the stack. E.g. for parameters <int, double, nString>
                 * we would expect the top value on the stack to be a C string, the second to top a double
                 * and the third to be an integer.
                 *
                 * \param state The Lua stack.
                 * \param tPtr A pointer to the tuple we wish to populate.
                 *
                 * \return True if all values are successfully popped, false otherwise.
                 */
                template<typename Type>
                bool rtryPopValues(Handle state, OUT std::tuple<Type>* tPtr) {
                    // Pop one value of type Type from stack and put in tuple.
                    return ValueMediator<Type>::tryPop(state, std::get<0>(*tPtr));
                }
                template<typename Type>
                bool rtryPopValues(Handle state, OUT std::tuple<Type&>* tPtr) {
                    // Attempt to pop off the pointer to the value, if successful copy the pointer into the tuple.
                    void* value = nullptr;
                    if (!ValueMediator<void*>::tryPop(state, value)) {
                        return false;
                    }
                    std::memcpy(tPtr, &value, sizeof(void*));
                    return true;
                }
                template<typename Type1, typename Type2, typename ...Types>
                bool rtryPopValues(Handle state, OUT std::tuple<Type1, Type2, Types...>* tPtr) {
                    // Pop all but the first value - we want this recursive approach to pop in reverse order of
                    // types provided.
                    std::tuple<Type2, Types...> tToPop{ ValueMediator<Type2, Types>::defaultValue()... };
                    if (!rtryPopValues<Type2, Types...>(state, &tToPop)) {
                        return false;
                    }

                    // Pop the first value.
                    std::tuple<Type1> tPopped{ ValueMediator<Type1>::defaultValue() };
                    if (!rtryPopValues<Type1>(state, &tPopped)) {
                        return false;
                    }

                    // Concatenate tuples and swap with the pointer provided.
                    *tPtr = std::tuple_cat(tPopped, tToPop);
                    return true;
                }
            }
            template<typename ...Types>
            std::tuple<Types...> popValues(Handle state) {
                // Initialise tuple with default values for each type of Parameters.
                std::tuple<Types...> values{ ValueMediator<Types>::defaultValue()... };
                impl::rpopValues(state, &values);

                return std::move(values);
            }
            template<typename ...Types>
            bool tryPopValues(Handle state, std::tuple<Types...>& values) {
                return impl::rtryPopValues(state, &values);
            }

            // Provides support for pushing and popping enums by converting to and from their underlying type.
            template<typename Type>
            struct ValueMediator<Type, typename std::enable_if<std::is_enum<Type>::value>::type> {
                // Gets the underlying type of the enum (e.g. ui8, ui16 etc.).
                using EnumType = typename std::underlying_type<Type>::type;
            public:
                static Type defaultValue() {
                    return static_cast<Type>(0);
                }

                static i32 getValueCount() {
                    return 1;
                }

                static i32 push(Handle state, Type value) {
                    return ValueMediator<EnumType>::push(state, static_cast<EnumType>(value));
                }

                static Type pop(Handle state) {
                    return static_cast<Type>(ValueMediator<EnumType>::pop(state));
                }

                static bool tryPop(Handle state, OUT Type& value) {
                    EnumType result;
                    // Try to pop the underlying enum type.
                    bool success = ValueMediator<EnumType>::tryPop(state, result);
                    // If successful, set value to the Type casted result of the pop.
                    if (success) {
                        value = static_cast<Type>(result);
                    }
                    // Return success of pop.
                    return success;
                }

                static Type retrieve(Handle state, ui32 index) {
                    return static_cast<Type>(ValueMediator<EnumType>::retrieve(state, index));
                }

                static bool tryRetrieve(Handle state, ui32 index, OUT Type& value) {
                    EnumType result;
                    // Try to pop the underlying enum type.
                    bool success = ValueMediator<EnumType>::tryRetrieve(state, index, result);
                    // If successful, set value to the Type casted result of the pop.
                    if (success) {
                        value = static_cast<Type>(result);
                    }
                    // Return success of pop.
                    return success;
                }
            };

            // Provides support for pushing and popping pointer types by casting to and from void*.
            template<typename Type>
            struct ValueMediator<Type*, typename std::enable_if<std::is_pointer<Type*>::value>::type> {
                using NoConstType = typename std::remove_const<Type>::type;
            public:
                static Type* defaultValue() {
                    return (Type*)nullptr;
                }

                static i32 getValueCount() {
                    return 1;
                }

                static i32 push(Handle state, Type* value) {
                    return ValueMediator<void*>::push(state, reinterpret_cast<void*>(const_cast<NoConstType*>(value)));
                }

                static Type* pop(Handle state) {
                    return reinterpret_cast<Type*>(ValueMediator<void*>::pop(state));
                }

                static bool tryPop(Handle state, OUT Type*& value) {
                    void* result;
                    // Try to pop the light user data.
                    bool success = ValueMediator<void*>::tryPop(state, result);
                    // If successful, set value to the Type* casted result of the pop.
                    if (success) {
                        value = reinterpret_cast<Type*>(result);
                    }
                    // Return success of pop.
                    return success;
                }

                static Type* retrieve(Handle state, ui32 index) {
                    return reinterpret_cast<Type*>(ValueMediator<void*>::retrieve(state, index));
                }

                static bool tryRetrieve(Handle state, ui32 index, OUT Type*& value) {
                    void* result;
                    // Try to pop the light user data.
                    bool success = ValueMediator<void*>::tryRetrieve(state, index, result);
                    // If successful, set value to the Type* casted result of the pop.
                    if (success) {
                        value = reinterpret_cast<Type*>(result);
                    }
                    // Return success of pop.
                    return success;
                }
            };

            // Provides support for pushing and popping reference types by casting to and from void*.
            template<typename Type>
            struct ValueMediator<Type&, typename std::enable_if<std::is_reference<Type&>::value>::type> {
            public:
                static Type& defaultValue() {
                    Type* temp = (Type*)nullptr;
                    return *temp;
                }

                static i32 getValueCount() {
                    return 1;
                }

                static i32 push(Handle state, Type& value) {
                    return ValueMediator<void*>::push(state, const_cast<void*>(&value));
                }

                static Type& pop(Handle state) {
                    return *((Type*)ValueMediator<void*>::pop(state));
                }

                static bool tryPop(Handle state, OUT Type& value) {
                    void* result;
                    // Try to pop the light user data.
                    bool success = ValueMediator<void*>::tryPop(state, result);
                    // If successful, set value to the Type casted result of the pop.
                    if (success) {
                        value = *(Type*)result;
                    }
                    // Return success of pop.
                    return success;
                }

                static Type& retrieve(Handle state, ui32 index) {
                    return *((Type*)ValueMediator<void*>::retrieve(state, index));
                }

                static bool tryRetrieve(Handle state, ui32 index, OUT Type& value) {
                    void* result;
                    // Try to pop the light user data.
                    bool success = ValueMediator<void*>::tryRetrieve(state, index, result);
                    // If successful, set value to the Type casted result of the pop.
                    if (success) {
                        value = *(Type*)result;
                    }
                    // Return success of pop.
                    return success;
                }
            };
        }
    }
}
namespace vscript = vorb::script;

#endif // VORB_USING_SCRIPT

#endif // !Vorb_Lua_ValueMediator_h__
