//
// lua/LFunction.h
// Vorb Engine
//
// Created by Matthew Marshall on 8 Nov 2018
// Based on Cristian Zaloj's prior script environment.
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file LFunction.h
 * \brief Classes wrapping Lua functions, providing call procedures for C++.
 */

#pragma once

#ifndef Vorb_Lua_LFunction_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Lua_LFunction_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#define SCRIPT_FUNCTION_TABLE "ScriptFunctions"

namespace vorb {
    namespace script {
        namespace lua {
            using Index = i32;

            // Forward declaration of RLFunction.
            template<typename ...ReturnTypes>
            class RLFunction;

            /*!
             * \brief Wrapper of a Lua function. Stores the environment handle, name of the wrapped function
             * and the index of the accessor of the wrapped function in the script function table in the 
             * lua registry. Provides a mechanism to call the wrapped lua function with arbitrary parameters 
             * and return values.
             */
            class LFunction {
                template<typename ...ReturnTypes>
                friend class RLFunction;
            public:
                LFunction() :
                    m_state(nullptr)
                { /* EMPTY */ }
                /*!
                 * \brief Constructor
                 *
                 * \param state The Lua stack.
                 * \param name The name of the function.
                 * \param index The index of the function in the Lua registry.
                 */
                LFunction(Handle state, const nString& name, Index index) :
                    m_state(state), m_name(name), m_index(index)
                { /* EMPTY */ }
                ~LFunction();

                /*!
                 * \brief Functions to call the wrapped lua function.
                 *
                 * \tparam Parameters The list of types of the parameters to be passed to the function.
                 *
                 * \param parameters The parameters passed to the function.
                 */
                template<typename ...Parameters>
                void call(Parameters... parameters) const;
                /*!
                 * \brief Functions to call the wrapped lua function.
                 *
                 * operator() is an alias of call.
                 *
                 * \tparam Parameters The list of types of the parameters to be passed to the function.
                 *
                 * \param parameters The parameters passed to the function.
                 */
                template<typename ...Parameters>
                void operator()(Parameters... parameters) const;

                /*!
                 * \brief Function to check if a given LFunction object is "Nil" - i.e. invalid.
                 *
                 * \return True if the instance is Nil, otherwise false.
                 */
                bool isNil() const;
                /*!
                 * \brief Equality operator.
                 *
                 * \return True if the instances are the same, false otherwise.
                 */
                bool operator==(const LFunction& rhs) const;

                /*!
                 * \brief Creates an RLFunction wrapper, which expects the wrapped lua function to return the types of ReturnTypes.
                 *
                 * \tparam ReturnTypes The list of types of the expected values to be returned from the Lua function.
                 *
                 * \return Instance of the RLFunction type, ready for calling.
                 */
                template<typename ...ReturnTypes>
                RLFunction<ReturnTypes...> withReturn() const;

                /*!
                 * \brief Get the name of the wrapped function.
                 *
                 * \return Name of the function this instance of LFunction wraps.
                 */
                const nString& getName() const { return m_name; }

                const static LFunction Nil; ///< A static "Nil" LFunction to vaidate other LFunctions against.
            protected:
                // TODO(Matthew): Unify error handling across the environment?
                /*!
                 * \brief Dumps errors from calling function into cout.
                 */
                void dumpErrors() const;

                Handle  m_state; ///< The Lua stack.
                nString m_name;  ///< The name of the function.
                Index   m_index; ///< The index of the function in Lua registry.
            };

            /*! \brief Variant of the wrapper that specifies expected return types. */
            template<typename ...ReturnTypes>
            class RLFunction : public LFunction {
            public:
                RLFunction() :
                    LFunction()
                { /* EMPTY */ }
                /*!
                 * \brief Constructor
                 *
                 * \param state The Lua stack.
                 * \param name The name of the function.
                 * \param index The index of the function in the Lua registry.
                 */
                RLFunction(Handle state, const nString& name, Index index) :
                    LFunction(state, name, index)
                { /* EMPTY */ }
                /*!
                 * \brief Copy Constructor
                 *
                 * \param lFunction The LFunction instance to copy.
                 */
                RLFunction(const LFunction& lFunction) :
                    LFunction(lFunction.m_state, lFunction.m_name, lFunction.m_index)
                { /* EMPTY */ }
                // We don't want to be unreferencing here as this class is simply a copy
                // of an LFunction with a return type.
                ~RLFunction()
                { /* EMPTY */ }


                /*!
                 * \brief Functions to call the wrapped lua function.
                 *
                 * \tparam Parameters The list of types of the parameters to be passed to the function.
                 *
                 * \param returnValues The tuple to place returned values into.
                 * \param parameters The parameters passed to the function.
                 */
                template<typename ...Parameters>
                void call(OUT std::tuple<ReturnTypes...>& returnValues, Parameters... parameters) const;
                /*!
                 * \brief Functions to call the wrapped lua function.
                 *
                 * operator() is an alias of call.
                 *
                 * \tparam Parameters The list of types of the parameters to be passed to the function.
                 *
                 * \param returnValues The tuple to place returned values into.
                 * \param parameters The parameters passed to the function.
                 */
                template<typename ...Parameters>
                void operator()(OUT std::tuple<ReturnTypes...>& returnValues, Parameters... parameters) const;
            };
        }
    }
}
namespace vscript  = vorb::script;

/*!
 * \brief Helper function to recursively push parameters onto the stack.
 *
 * \tparam Parameter The type of the parameter to push.
 *
 * \param state The Lua stack.
 * \param parameter The parameter to push.
 */
template<typename Parameter>
i32 pushParams(Handle state, Parameter parameter) {
    return vscript::lua::ValueMediator<Parameter>::push(state, parameter);
}
/*!
 * \brief Helper function to recursively push parameters onto the stack.
 *
 * \tparam Parameter The type of the first parameter to push.
 * \tparam Parameters The type of the rest of the parameters to push.
 *
 * \param state The Lua stack.
 * \param parameter The parameter to push first.
 * \param parameters The rest of the parameters to push.
 */
template<typename Parameter, typename ...Parameters>
i32 pushParams(Handle state, Parameter parameter, Parameters... parameters) {
    i32 result = vscript::lua::ValueMediator<Parameter>::push(state, parameter);
    return result + pushParams<Parameters...>(state, parameters...);
}
/*!
 * \brief Helper function to recursively push parameters onto the stack.
 *
 * \param state The Lua stack.
 */
inline i32 pushParams(Handle state) {
    return 0;
}

template<typename ...Parameters>
inline void vscript::lua::LFunction::call(Parameters... parameters) const {
    // Put the script function table in the lua registry on to the top of the lua stack.
    lua_getfield(m_state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);
    // Get a raw accessor to the wrapped script function.
    lua_rawgeti(m_state, -1, m_index);

    // Push the parameters of this call onto the stack.
    pushParams(m_state, parameters...);

    // Call the function with the provided parameters.
    if (lua_pcall(m_state, sizeof...(Parameters), 0, 0) != 0) {
        // Print any errors that arose.
        dumpErrors();
    }

    // Pop the script function table off the stack.
    lua_pop(m_state, 1);
}

template<typename ...Parameters>
inline void vscript::lua::LFunction::operator()(Parameters... parameters) const {
    call(std::forward<Parameters>(parameters)...);
}

template<typename ...ReturnTypes>
inline vscript::lua::RLFunction<ReturnTypes...> vscript::lua::LFunction::withReturn() const {
    return RLFunction<ReturnTypes...>(*this);
}

template<typename ...ReturnTypes>
template<typename ...Parameters>
inline void vscript::lua::RLFunction<ReturnTypes...>::call(OUT std::tuple<ReturnTypes...>& returnValues, Parameters ...parameters) const {
    // Put the script function table in the lua registry on to the top of the lua stack.
    lua_getfield(m_state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);
    // Get a raw accessor to the wrapped script function.
    lua_rawgeti(m_state, -1, m_index);

    // Push the parameters of this call onto the stack.
    pushParams(m_state, parameters...);

    // Store the depth of the stack before calling the lua function.
    i32 initialDepth = lua_gettop(m_state);

    // Call the function with the provided parameters.
    if (lua_pcall(m_state, sizeof...(Parameters), getTotalValueCount<ReturnTypes...>(), 0) != 0) {
        // Print any errors that arose.
        dumpErrors();
    }

    // Try to pop all of the expected return values off of the stack.
    if (!tryPopValues<ReturnTypes...>(m_state, returnValues)) {
        // TODO(Matthew): Unify error handling across the environment?
        // Handle the failure.
        std::cout << "Uh oh, could not get the return values as expected." << std::endl;

        // Calculate the change in the stack depth.
        i32 deltaDepth = lua_gettop(m_state) - initialDepth;
        // Pop the remaining returned values off the stack.
        lua_pop(m_state, deltaDepth);
    }

    // Pop the script function table off the stack.
    lua_pop(m_state, 1);
}

template<typename ...ReturnTypes>
template<typename ...Parameters>
inline void vscript::lua::RLFunction<ReturnTypes...>::operator()(OUT std::tuple<ReturnTypes...>& returnValues, Parameters ...parameters) const {
    call(returnValues, std::forward<Parameters>(parameters)...);
}

#endif // VORB_USING_SCRIPT

#endif // !Vorb_Lua_LFunction_h__
