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

#include "Event.hpp"

namespace vorb {
    namespace script {
        class IEnvironment {
            using CFunctionList = std::vector<std::unique_ptr<DelegateBase>>;
            using EventAdder    = void(*)();
            struct EventData {
                EventBase* event;
                EventAdder adder;
            }
            using EventList = std::unordered_map<nString, EventData>;
        public:
            /*!
             * \brief Load in a script from the provided filepath.
             *
             * \param filepath The filepath from which to load the script.
             */
            virtual bool load(const vio::Path& filepath) = 0;

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
            bool registerEvent(std::string name, EventData eventData) {
                return m_events.insert({ name, eventData } }).second;
            }
        protected:
            CFunctionList m_cFunctions;
            EventLsit     m_events;
        };
    }
}
namespace vscript = vorb::script;

#endif // VORB_USING_SCRIPT

#endif // !Vorb_IEnvironment_h__
