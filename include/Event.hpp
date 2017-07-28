//
// Event.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Events.hpp
* @brief C#-style events.
*/

#pragma once

#ifndef Vorb_Event_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Event_hpp__
//! @endcond

#include "Delegate.hpp"

typedef const void* EventSender;

// An event that invokes methods taking certain arguments. Requires an STL list implementation
template<template<typename> class TList, typename... Params>
class Event {
public:
    typedef Delegate<void, EventSender, Params...> Listener; // Callback delegate type

    // Create an event with a sender attached to it
    Event(EventSender sender = nullptr) :
        m_sender(sender) {
        // Empty
    }

    // Events can't be copied.
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    // Reset the sender value of this event to another. Listeners may be expecting a constant sender, so
    // using this could cause weird side effects.
    void setSender(EventSender s) {
        m_sender = s;
    }

    // Call all bound methods with proper parameters passed in
    void send(Params... p) {
        size_t sz = m_funcs.size();
        // Test against both a cached local size and queried size because sometimes the events listeners
        // list increases.
        for (size_t i = 0; (i < sz) && (i < m_funcs.size()); i++) {
            m_funcs[i](m_sender, p...);
        }
    }
    void operator()(Params... p) {
        this->send(p...);
    }

    // Add a function to this event. It will take ownership of the delegate and perform all destruction.
    Event& addMove(Listener&& f) {
        m_funcs.emplace_back(std::move(f));
        return *this;
    }

    // Add a function to this event whilst allowing chaining
    Event& operator+=(Listener&& f) {
        return addMove(std::move(f));
    }

    // Remove a function (just one) from this event
    Event& remove(const Listener& f) {
        auto fLoc = m_funcs.begin();
        while (fLoc != m_funcs.end()) {
            if (*fLoc == f) {
                break;
            }
            ++fLoc;
        }
        if (fLoc == m_funcs.end()) {
            // We couldn't find that listener
            return *this;
        }

        // Remove one instance of that listener
        m_funcs.erase(fLoc);
        return *this;
    }
    // Remove a function (just one) from this event whilst allowing chaining
    Event& operator-=(const Listener& f) {
        return remove(f);
    }
private:
    EventSender m_sender; // Event owner
    TList<Listener> m_funcs; // List of bound functions (subscribers)
};

#endif // !Vorb_Event_hpp__
