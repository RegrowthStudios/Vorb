//
// IEnvironment.h
// Vorb Engine
//
// Created by Matthew Marshall on 8 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file IEnvironment.h
 * \brief Interface for scripting environments.
 * 
 * Script environments act like databases. They track and manage
 * all scripted function pointers. They also provide functions
 * to register C++ callbacks for scripts to use, in addition to
 * functions to register events for script functions to subscribe to.
 */

// TODO(Matthew): Investigate how to apply application control over the amount of time dedicated to executing scripts, including 
//                time spent executing script callbacks subscribed to events.
// TODO(Matthew): Simplify templating when (if) we shift to C++17.

#pragma once

#ifndef Vorb_IEnvironment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IEnvironment_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include <unordered_map>

#include "../Event.hpp"
#include "../io/Path.h"

namespace vorb {
    namespace script {
        using GenericScriptFunction = void(*)();
        using GenericCFunction      = void(*)();

        template <typename EnvironmentImpl>
        class IEnvironment {
            using CFunctionList = std::vector<std::unique_ptr<DelegateBase>>;
            using EventAdder    = void(EnvironmentImpl::*)(GenericScriptFunction, EventBase*);
            struct EventData {
                EventBase* event;
                EventAdder adder;
            };
            using EventList = std::unordered_map<nString, EventData>;
        public:
            // TODO(Matthew): Initialisation and disposal.

            /*!
             * \brief Load in a script from the provided filepath.
             *
             * \param filepath The filepath from which to load the script.
             */
            bool load(const vio::Path& filepath) {
                static_cast<EnvironmentImpl*>(this)->load(filepath);
            }

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
            bool registerEvent(const nString& name, EventData eventData) {
                return m_events.insert({ name, eventData }).second;
            }
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
                return m_events.insert({ name, { static_cast<EventBase*>(event), &EnvironmentImpl::addScriptFunctionToEvent<Parameters...> } }).second;
            }

            /*!
             * \brief Adds the provided LFunction to the provided Event.
             *
             * \tparam Parameters The list of parameter types provided by the Event on trigger.
             *
             * \param scriptFunction The Lua function to register to the event.
             * \param eventBase The event to subscribe the Lua function to.
             */
            template <typename ...Parameters>
            void addScriptFunctionToEvent(GenericScriptFunction scriptFunction, EventBase* eventBase) {
                static_cast<EnvironmentImpl*>(this)->addScriptFunctionToEvent(scriptFunction, eventBase);
            }

            /*!
             * \brief Add a C++ delegate as a hook with name "name" for Lua scripts to call.
             *
             * Note that this instance is here to act as a fake virtualisation of member template.
             *
             * \param name The name of the delegate to add.
             * \param function The delegate to add.
             */
            template<typename ReturnType, typename ...Parameters, typename DelegateType = Delegate<ReturnType, Parameters...>>
            void addCDelegate(const nString& name, DelegateType&& delegate) {
                static_cast<EnvironmentImpl*>(this)->addCDelegate(name, std::move(delegate));
            }

            /*!
             * \brief Adds the given value to the top of the lua stack.
             *
             * Note that this instance is here to act as a fake virtualisation of member template.
             *
             * \param name The key to give the value to be added to the Lua stack.
             * \param value The value to add to the Lua stack.
             */
            template<typename Type>
            bool addValue(const nString& name, Type value) {
                return static_cast<EnvironmentImpl*>(this)->addValue(name, std::move(value));
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
             * \brief Sets a series of embedded lua tables to the top of the lua stack.
             *
             * Note that this instance is here to act as a fake virtualisation of member template.
             *
             * \param namespaces The namespaces to set.
             */
            template<typename ...Namespaces>
            void setNamespaces(Namespaces... namespaces) {
                static_cast<EnvironmentImpl*>(this)->setNamespaces(std::forward<Namespaces>(namespaces)...);
            }
        protected:
            i32              m_maxScriptLength; ///< The maximum length a single script may be.
            CFunctionList    m_cFunctions;      ///< List of C++ functions registered with the script environment.
            EventList        m_events;          ///< List of events registered with the script environment.
            AutoDelegatePool m_listenerPool;    ///< Manages Lua functions subscribed to events.
        };
    }
}
namespace vscript = vorb::script;

#endif // VORB_USING_SCRIPT

#endif // !Vorb_IEnvironment_h__
