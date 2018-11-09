//
// lua/CFunction.h
// Vorb Engine
//
// Created by Matthew Marshall on 8 Nov 2018
// Based on Cristian Zaloj's prior script environment.
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file CFunction.h
 * \brief Helper functions for handling C++ functions invokable by Lua scripts.
 */

#pragma once

#ifndef Vorb_Lua_CFunction_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Lua_CFunction_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include "ValueMediator.h"
#include "../../Delegate.hpp"
#include "../../IndexSequence.hpp"

namespace vorb {
    namespace script {
        namespace lua {
            namespace CFunction {
                using Type   = int(*)(Handle); ///< The type of callable CFunctions from Lua - we use a wrapper and delegates to call functions with other signatures.

                namespace impl {
                    /*!
                    * \brief Invokes the delegate, passing it the parameters stored in the passed in tuple.
                    *
                    * We used the index sequence trick to unpack the tuple.
                    *
                    * \tparam ReturnType The return type of the delegate.
                    * \tparam Delegate The delegate's type.
                    * \tparam Tuple The tuple's type.
                    * \tparam Indices The indices used by the index sequence trick.
                    *
                    * \param delegate The delegate to invoke.
                    * \param tuple The tuple of parameters to pass to the delegate.
                    */
                    template<typename ReturnType, typename Delegate, typename Tuple, size_t ...Indices>
                    ReturnType invokeDelegate(Delegate delegate, Tuple& tuple, index_sequence<Indices...>) {
                        return delegate->invoke(std::get<Indices>(tuple)...);
                    }

                    /*!
                    * \brief Series of functions to handle invocation of the delegate depending on combination of return type and parameters expected.
                    *
                    * These functions fetch any parameters expected, and pushes any returned values onto the lua stack.
                    *
                    * \tparam ReturnType The return type of the delegate.
                    * \tparam Parameters The parameter types expected by the delegate.
                    *
                    * \param state The Lua state object.
                    * \param delegate The delegate to invoke.
                    */
                    template<typename ReturnType, typename ...Parameters>
                    i32 handleInvocation(Handle state, Delegate<ReturnType, Parameters...>* delegate) {
                        // Pop parameters to be provided to the delegate off of the Lua stack.
                        std::tuple<Parameters...> parameters = popValues<Parameters...>(state);

                        // Invoke delegate, unpacking the tuple of parameters using the index sequence trick.
                        ReturnType ret = invokeDelegate<ReturnType>(delegate, parameters, index_sequence_for<Parameters...>());

                        // Push the return value of the delegate onto the stack and return the number of items on the stack that the return value takes up.
                        return ValueMediator<ReturnType>::push(state, ret);
                    }
                    template<typename ReturnType>
                    inline i32 handleInvocation(Handle state, Delegate<ReturnType>* delegate) {
                        // Just invoke delegate, no parameters to fetch off lua stack.
                        ReturnType ret = delegate->invoke();

                        // Push the return value of the delegate onto the stack and return the number of items on the stack that the return value takes up.
                        return ValueMediator<ReturnType>::push(state, ret);
                    }
                    template<typename = void, typename ...Parameters>
                    inline i32 handleInvocation(Handle state, Delegate<void, Parameters...>* delegate) {
                        // Pop parameters to be provided to the delegate off of the Lua stack.
                        std::tuple<Parameters...> parameters = popValues<Parameters...>(state);

                        // Invoke delegate, unpacking the tuple of parameters using the index sequence trick.
                        invokeDelegate<void>(delegate, parameters, index_sequence_for<Parameters...>());

                        // Void return, 0 items added to lua stack.
                        return 0;
                    }
                    template<typename = void>
                    inline i32 handleInvocation(Handle, Delegate<void>* delegate) {
                        // Just invoke delegate, no parameters to fetch off lua stack.
                        delegate->invoke();

                        // Void return, 0 items added to lua stack.
                        return 0;
                    }

                    /*!
                    * \brief This function acts as a wrapper callable from Lua. It grabs the captured delegate from the Lua stack and hands it off for invocation.
                    *
                    * We take a function pointer to this object, storing it for later use, when we call fromDelegate.
                    *
                    * \tparam ReturnType The return type of the delegate.
                    * \tparam Parameters The parameter types expected by the delegate.
                    *
                    * \param state The Lua state object.
                    */
                    template<typename ReturnType, typename ...Parameters>
                    int luaDelegateWrapper(Handle state) {
                        // Define the type of delegate that is being wrapped.
                        using DelegateType = Delegate<ReturnType, Parameters...>;

                        // Get the captured delegate.
                        DelegateType* delegate = (DelegateType*)lua_touserdata(state, lua_upvalueindex(1));

                        // Pass off to invocation handler.
                        return handleInvocation(state, delegate);
                    }
                }

                /*!
                * \brief Returns a function pointer suitable for calling from Lua that in turn invokes the delegate associated with it.
                *
                * Note that we wrap this in a functor that captures the associated delegate.
                *
                * \tparam ReturnType The type returned by the function pointer.
                * \tparam Parameters The parameters needed to be passed to the function pointer.
                */
                template<typename ReturnType, typename ...Parameters>
                Type fromDelegate() {
                    return impl::luaDelegateWrapper<ReturnType, Parameters...>;
                }
            }
        }
    }
}
namespace vscript  = vorb::script;

#endif // VORB_USING_SCRIPT

#endif // !Vorb_Lua_CFunction_h__
