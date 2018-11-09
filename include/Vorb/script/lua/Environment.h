//
// lua/Environment.h
// Vorb Engine
//
// Created by Matthew Marshall on 8 Nov 2018
// Based on Cristian Zaloj's prior script environment.
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file Environment.h
 * \brief Lua implementation of the script environment.
 */

#pragma once

#ifndef Vorb_Lua_Environment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Lua_Environment_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include <lua.hpp>

namespace vorb {
    namespace script {
        namespace lua {
            using Handle = lua_State*; ///< The Lua state handle.
        }
    }
}

#include "CFunction.hpp"
#include "../IEnvironment.hpp"

namespace vorb {
    namespace script {
        namespace lua {
            class LFunction;

            /*!
             * \brief Adds the provided LFunction to the provided Event.
             *
             * \tparam Parameters The list of parameter types provided by the Event on trigger.
             *
             * \param pool The pool to use for resource managing.
             * \param lFunction The Lua function to register to the event.
             * \param eventBase The event to subscribe the Lua function to.
             */
            template <typename ...Parameters>
            void addLFunctionToEvent(AutoDelegatePool& pool, GenericScriptFunction scriptFunction, EventBase* eventBase);

            class Environment : public IEnvironment {
                using LFunctionList = std::unordered_map<nString, LFunction*>;
            public:
                Environment();
                Environment(const nString& filepath);
                ~Environment();

                /*!
                * \brief Load in a script from the provided filepath.
                *
                * \param filepath The filepath from which to load the script.
                */
                virtual bool load(const vio::Path& filepath) override;

                /*!
                 * \brief Register an event with the script environment.
                 *
                 * The notion behind this is that we are able to add the subscriber 
                 * to the appropriate event using a generic function pointer. This
                 * way we need not use any templating here.
                 *
                 * \param name The name of the event for use by lua scripts when subscribing.
                 * \param eventData The event to be registered with a pointer to the 
                 *                  function which adds subscribers to the event.
                 */
                template <typename ...Parameters>
                bool registerEvent(const nString& name, Event<Parameters...>* event) {
                    return m_events.insert({ name, { static_cast<EventBase*>(event), &addLFunctionToEvent<Parameters...> } }).second;
                }

                /*!
                 * \brief Set the max length each script may be.
                 *
                 * \param length The max length.
                 */
                void setMaxScriptLength(i32 length) { m_maxScriptLength = length; }

                /*!
                 * \brief Get the max length each script may be.
                 *
                 * \return The max length.
                 */
                i32 getMaxScriptLength() { return m_maxScriptLength; }

                /*!
                * \brief Add a C++ function as a hook with name "name" for Lua scripts to call.
                *
                * \param name The name of the function to add.
                * \param function The function to add.
                */
                void addCFunction(const nString& name, CFunction::Type function);
                /*!
                * \brief Add a C++ function that captures upvalueCount many upvalues - variables accessible at every call of the function.
                *
                * \param name The name of the function to add as a closure.
                * \param upvalueCount The number of values at the top of the Lua stack to capture in this closure.
                * \param function The function to add.
                */
                void addCClosure(const nString& name, ui8 upvalueCount, CFunction::Type function);
                /*!
                * \brief Add a C++ delegate as a hook with name "name" for Lua scripts to call.
                *
                * \param name The name of the delegate to add.
                * \param function The delegate to add.
                */
                template<typename ReturnType, typename ...Parameters, typename DelegateType = Delegate<ReturnType, Parameters...>>
                void addCDelegate(const nString& name, DelegateType&& delegate);

                /*!
                * \brief Add a Lua function to the list of registered Lua functions.
                *
                * \param lFunction The Lua function to register.
                *
                * \return True on successful register, false otherwise.
                */
                bool addLFunction(LFunction* lFunction);
                /*!
                * \brief Subscribe a Lua callback to the named event.
                *
                * \param eventName The event to subscribe the Lua function to.
                * \param lFunction The Lua function to register.
                *
                * \return True on successful subscription, false otherwise.
                */
                bool subscribeLFunction(const nString& eventName, LFunction* lFunction);

                /*!
                * \brief Retrieves the named Lua function from the registered list.
                *
                * \param lFunction The Lua function to register.
                *
                * \return Pointer to the function obtained.
                */
                LFunction* getLFunction(const nString& name);

                /*!
                * \brief Adds the given value to the top of the lua stack.
                *
                * \param name The key to give the value to be added to the Lua stack.
                * \param value The value to add to the Lua stack.
                */
                template<typename Type>
                bool addValue(const nString& name, Type value);

                /*!
                * \brief Sets the global lua table to the top of the lua stack.
                */
                void setGlobalNamespace();
                /*!
                * \brief Sets a series of embedded lua tables to the top of the lua stack.
                *
                * \param namespaces The namespaces to set.
                */
                template<typename ...Namespaces>
                void setNamespaces(Namespaces... namespaces);
            protected:
                VORB_NON_COPYABLE(Environment);

                /*!
                * \brief Recursively pushes tables onto the top of the lua stack.
                *
                * \param namespace The namespaces to push now.
                * \param namespaces The namespaces to push next.
                */
                template<typename Namespace, typename ...Namespaces>
                void pushNamespaces(Namespace namespace_, Namespaces... namespaces);
                /*!
                * \brief Specialisation to complete this recursive loop.
                */
                inline void pushNamespaces() {
                    // Empty
                }
                /*!
                * \brief Pushes the specific namespace given to the Lua stack.
                *
                * \param namespace The namespaces to push.
                */
                void pushNamespace(const nString& namespace_);

                Handle        m_state;           ///< The Lua state handle.
                i32           m_namespaceDepth;  ///< The depth of namespace we are currently at.
                i32           m_maxScriptLength; ///< The maximum length a single script may be.
                LFunctionList m_lFunctions;      ///< The list of Lua functions registered with this environment.
            };

            /*!
            * \brief Dumps the lua stack to cout.
            *
            * //TODO(Matthew): Yield choice of output mode to caller?
            */
            void dumpStack();

            /*!
             * \brief Makes an LFunction instance from a registering Lua function.
             *
             * \param state The Lua state handle managing the script the registering function is from.
             *
             * \return The number of elements returned to the Lua stack.
             */
            int makeLFunction(Handle state);
            /*!
             * \brief Makes an LFunction instance of the registering Lua function, subscribing it to the specified event.
             *
             * \param state The Lua state handle managing the script the registering function is from.
             *
             * \return The number of elements returned to the Lua stack.
             */
            int makeLCallback(Handle state);
        }
    }
}
namespace vscript = vorb::script;

template<typename ReturnType, typename ...Parameters, typename DelegateType>
inline void vscript::lua::Environment::addCDelegate(const nString& name, DelegateType&& delegate) {
    // Create copy of delegate and store it.
    m_cFunctions.emplace_back(new DelegateType(delegate));

    // Get function to wrap delegate.
    CFunction::Type function = CFunction::fromDelegate<ReturnType, Parameters...>();

    // Push pointer to delegate onto stack and then make a closure of the wrapping function that captures the delegate.
    ValueMediator<void*>::push(m_state, m_cFunctions.back().get());
    addCClosure(name, 1, function);
}

template<typename Type>
inline bool vscript::lua::Environment::addValue(const nString& name, Type value) {
    // See if a field already exists with the key of this value.
    lua_getfield(m_state, -1, name.c_str());
    // If it doesn't, we can add it.
    if (lua_isnil(m_state, -1)) {
        // Pop the result of looking for a populated key.
        lua_pop(m_state, 1);
        // Push value and set a field of key name to hold that value.
        ValueMediator<Type>::push(m_state, value);
        lua_setfield(m_state, -2, name.c_str());
        // Success
        return true;
    }
    // Pop the result of looking for a populated key.
    //     Note this is different to the case in pushNamespace as while in that case finding a
    //     field already exists or creating one are both successes, in the case of adding a value
    //     we have failed if the field we want to add it to already exists.
    lua_pop(m_state, 1);
    // Fail
    return false;
}

template<typename ...Namespaces>
inline void vscript::lua::Environment::setNamespaces(Namespaces... namespaces) {
    // Pop any namespaces currently set.
    if (m_namespaceDepth > 0) {
        lua_pop(m_state, m_namespaceDepth);
    }
    // Push the new namespaces onto stack.
    pushNamespaces(namespaces...);
    // Set the new namespace depth.
    m_namespaceDepth = sizeof...(Namespaces);
}

template<typename Namespace, typename ...Namespaces>
inline void vscript::lua::Environment::pushNamespaces(Namespace namespace_, Namespaces... namespaces) {
    // Recursive solution to unpack variadic namespaces.
    // Should be good enough solution given the insane depth of namespaces needed to cause stack overflow.
    pushNamespace(namespace_);
    pushNamespaces(namespaces...);
}

template <typename ...Parameters>
void vscript::lua::addLFunctionToEvent(AutoDelegatePool& pool, GenericScriptFunction scriptFunction, EventBase* eventBase) {
    Event<Parameters...>* event     = static_cast<Event<Parameters...>*>(eventBase);
    LFunction*            lFunction = static_cast<LFunction*>(scriptFunction);

    pool.addAutoHook(*event, *lFunction);
}

#endif // VORB_USING_SCRIPT

#endif // !Vorb_Lua_Environment_h__
