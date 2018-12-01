//
// Event.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 7 Nov 2018
// Based on the original implementation by Cristian Zaloj
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file Event.hpp
 * \brief Provides a definition of events, which can be subscribed to via delegates, and may be
 * triggered, calling each subscriber in turn - in order of subscription.
 */

#pragma once

#ifndef Vorb_Event_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Event_h__
//! @endcond

#include <algorithm>
#include <vector>

#include "decorators.h"
#include "Delegate.hpp"

/************************************************************************\
 *                     The Event Implementation                         *
\************************************************************************/

// Sender is the object responsible for handling the event, and is stored simply as a void pointer; making things
// easier for us and harder for users to do stupid things.
using Sender = const void*;

// Event base class, provides all aspects of the Event class that don't depend on templated parameters.
class EventBase {
public:
    /*!
     * \brief Default Constructor
     *
     * \param sender The owner of the event.
     */
    EventBase(Sender sender = nullptr) :
        m_sender(sender)
    { /* Empty */ }

    /*!
     * \brief Allows tranferring "ownership" of an event. Use with caution!
     *
     * \param sender The owner of the event.
     */
    void setSender(Sender sender) {
        m_sender = sender;
    }
protected:
    Sender m_sender;
};
// The standard event class, provides the most trivial event system in which subscribers are invoked in order of
// subscription and all subscribers get processed.
template <typename ...Parameters>
class Event : public EventBase {
public:
    using Subscriber  = Delegate<void, Sender, Parameters...>;
    using Subscribers = std::vector<Subscriber>;
    /*!
     * \brief Default Constructor
     *
     * \param sender The owner of the event.
     */
    Event(Sender sender = nullptr) :
        EventBase(sender),
        m_triggering(false)
    { /* Empty */ }
    /*!
     * \brief Copies the event and subscribers. Ownership must be considered.
     * \warning If delegates are not refcounted, whichever event will have the shortest lifetime needs to be neutered.
     *
     * \param event The event to copy.
     */
    Event(const Event& event) {
        m_sender       = event.m_sender;
        m_subscribers  = event.m_subscribers;
        m_removalQueue = event.m_removalQueue;
        m_triggering   = false;
    }
    /*!
     * \brief Moves the event. Ownership is guaranteed to be transferred.
     *
     * \param event The event to copy.
     */
    Event(Event&& event) {
        m_sender       = event.m_sender;
        m_subscribers  = std::move(event.m_subscribers);
        m_removalQueue = std::move(event.m_removalQueue);
        m_triggering   = false;
    }

    /*!
     * \brief Copies the event and subscribers. Ownership must be considered.
     * \warning If delegates are not refcounted, whichever event will have the shortest lifetime needs to be neutered.
     *
     * \param event The event to copy.
     *
     * \return The event that has been copied to.
     */
    Event& operator=(const Event& event) {
        m_sender       = event.m_sender;
        m_subscribers  = event.m_subscribers;
        m_removalQueue = event.m_removalQueue;
        m_triggering   = false;

        return *this;
    }
    /*!
     * \brief Moves the event. Ownership is guaranteed to be transferred.
     *
     * \param event The event to move.
     *
     * \return The event that has been copied to.
     */
    Event& operator=(Event&& event) {
        m_sender       = event.m_sender;
        m_subscribers  = std::move(event.m_subscribers);
        m_removalQueue = std::move(event.m_removalQueue);
        m_triggering   = false;

        return *this;
    }

    /*!
     * \brief Neuters all subscribers of the event.
     */
    void neuter() {
#if REFCOUNT_DELEGATES != 1
        for (auto& subscriber : m_subscribers) {
            subscriber.neuter();
        }
#endif
    }

#if REFCOUNT_DELEGATES != 1
    /*!
     * \brief Adds a subscriber to the event.
     *
     * Specialisation as on copy we need to neuter the delegate
     * object if it isn't ref counted.
     *
     * \param subscriber The subscriber to add to the event.
     * \param own Whether the event owns the subscriber it.
     */
    void add(const Subscriber& subscriber, bool own = false) {
        m_subscribers.emplace_back(subscriber);

        if (!own) m_subscribers.back().neuter();
    }
    /*!
     * \brief Adds a subscriber to the event.
     *
     * operator+= is an alias of add, where the subscriber is taken to not
     * be owned (i.e. own = false).
     *
     * Specialisation as on copy we need to neuter the delegate
     * object if it isn't ref counted.
     *
     * \param subscriber The subscriber to add to the event.
     */
    void operator+=(const Subscriber& subscriber) {
        m_subscribers.emplace_back(subscriber);

        m_subscribers.back().neuter();
    }
#endif
    /*!
     * \brief Adds a subscriber to the event.
     *
     * \param subscriber The subscriber to add to the event.
     * \param stub This does nothing, it just ensures no errors if ref counting is enabled.
     */
    void add(Subscriber&& subscriber, bool = true) {
        m_subscribers.emplace_back(std::forward<Subscriber>(subscriber));
    }
    /*!
     * \brief Adds a subscriber to the event.
     *
     * operator+= is an alias of add, where the subscriber is taken to not
     * be owned (i.e. own = false).
     *
     * \param subscriber The subscriber to add to the event.
     */
    void operator+=(Subscriber&& subscriber) {
        add(std::forward<Subscriber>(subscriber));
    }

    /*!
     * \brief Adds a functor as a subscriber to this event.
     *
     * \tparam Functor The type of the functor to be subscribed.
     * \param functor The functor to be subscribed.
     * 
     * \return The created functor delegate.
     */
    template <typename Functor>
    CALLER_DELETE Subscriber* addFunctor(Functor&& functor) {
        add(makeFunctor(std::forward<Functor>(functor)));

        return &m_subscribers.back();
    }
    /*!
     * \brief Adds a functor as a subscriber to this event.
     *
     * This is a specialisation to deal with edge cases where we can't deduce from overloaded functions,
     * for example if an operator() is template typename overloaded. The more obvious case is const, non-const
     * overloading.
     *
     * \tparam Functor The type of the functor to be subscribed.
     * \param functor The functor to be subscribed.
     * 
     * \return The created functor delegate.
     */
    template <typename Functor>
    CALLER_DELETE Subscriber* addConstFunctor(Functor&& functor) {
        add(makeConstFunctor<void, Sender, Parameters...>(std::forward<Functor>(functor)));

        return &m_subscribers.back();
    }
    /*!
     * \brief Adds a functor as a subscriber to this event.
     *
     * This is a specialisation to deal with edge cases where we can't deduce from overloaded functions,
     * for example if an operator() is template typename overloaded. The more obvious case is const, non-const
     * overloading.
     *
     * \tparam Functor The type of the functor to be subscribed.
     * \param functor The functor to be subscribed.
     * 
     * \return The created functor delegate.
     */
    template <typename Functor, typename DecayedFunctor = typename std::decay<Functor>::type>
    CALLER_DELETE Subscriber* addNonConstFunctor(Functor&& functor) {
        add(makeNonConstFunctor<void, Sender, Parameters...>(std::forward<DecayedFunctor>(functor)));

        return &m_subscribers.back();
    }

    /*!
     * \brief Removes a subscriber from the event (only the first instance found is removed).
     *
     * \param subscriber The subscriber to remove from the event.
     */
    void remove(const Subscriber& subscriber) {
        // We don't want to be invalidating iterators by removing subscribers mid-trigger.
        if (m_triggering) {
            m_removalQueue.emplace_back(subscriber);
        } else {
            const auto& it = std::find(m_subscribers.begin(), m_subscribers.end(), subscriber);
            if (it != m_subscribers.end()) {
                m_subscribers.erase(it);
            }
        }
    }
    /*!
     * \brief Removes a subscriber from the event (only the first instance found is removed).
     *
     * operator-= is an alias of remove
     *
     * \param subscriber The subscriber to remove from the event.
     */
    void operator-=(const Subscriber& subscriber) {
        remove(subscriber);
    }

    /*!
     * \brief Triggers the event, invoking all subscribers.
     *
     * \param parameters The parameters to pass to subscribers.
     */
    void trigger(Parameters... parameters) {
        // We don't want to be invalidating iterators by removing subscribers mid-trigger.
        m_triggering = true;

        for (auto& subscriber : m_subscribers) {
            subscriber(m_sender, parameters...);
        }

        m_triggering = false;

        // Remove any subscribers that requested to be unsubscribed during triggering.
        for (auto& unsubscriber : m_removalQueue) {
            const auto& it = std::find(m_subscribers.begin(), m_subscribers.end(), unsubscriber);
            if (it != m_subscribers.end()) {
                m_subscribers.erase(it);
            }
        }
    }
    /*!
     * \brief Triggers the event, invoking all subscribers.
     *
     * operator() is an alias of trigger
     *
     * \param parameters The parameters to pass to subscribers.
     */
    void operator()(Parameters... parameters) {
        trigger(std::forward<Parameters>(parameters)...);
    }
protected:
    Subscribers m_subscribers;
    Subscribers m_removalQueue;
    bool        m_triggering;
};

/**************************************************************************\
 *                   AutoDelegatePool Implementation                      *
\**************************************************************************/

/*!
 * \brief Manages delegates created from functors subscribed to arbitrary events, ensuring proper clean-up.
 */
class AutoDelegatePool {
public:
    using Deletor     = Delegate<void>;       ///< A deletion function prototype.
    using DeletorList = std::vector<Deletor>; ///< A container of deleters.

    /*!
     * \brief Automatically dispose if we hit destructor - allows RAII usage of this class.
     */
    virtual ~AutoDelegatePool() {
        dispose();
    }

    /*!
     * \brief Binds the provided functor to an event, and stores a deletor for later clean-up.
     *
     * \tparam Functor The type of the functor provided.
     * \tparam Parameters The parameters the event provides when triggered.
     * \param event The event to subscribe the provided functor to.
     * \param functor The functor to subscribe to the provided event.
     */
    template<typename Functor, typename... Parameters>
    void addAutoHook(Event<Parameters...>& event, Functor&& functor) {
        auto delegate = event.template addFunctor<Functor>(std::forward<Functor>(functor));

        m_deletionFunctions.emplace_back(makeFunctor([delegate, &event] () {
            event -= *delegate;
        }));
    }
    /*!
     * \brief Binds the provided functor to an event, and stores a deletor for later clean-up.
     *
     * This is a specialisation to deal with edge cases where we can't deduce from overloaded functions,
     * for example if an operator() is template typename overloaded. The more obvious case is const, non-const
     * overloading.
     *
     * \tparam Functor The type of the functor provided.
     * \tparam Parameters The parameters the event provides when triggered.
     * \param event The event to subscribe the provided functor to.
     * \param functor The functor to subscribe to the provided event.
     */
    template<typename Functor, typename... Parameters>
    void addConstAutoHook(Event<Parameters...>& event, Functor&& functor) {
        auto delegate = event.template addConstFunctor<Functor>(std::forward<Functor>(functor));

        m_deletionFunctions.emplace_back(makeFunctor([delegate, &event] () {
            event -= *delegate;
        }));
    }
    /*!
     * \brief Binds the provided functor to an event, and stores a deletor for later clean-up.
     *
     * This is a specialisation to deal with edge cases where we can't deduce from overloaded functions,
     * for example if an operator() is template typename overloaded. The more obvious case is const, non-const
     * overloading.
     *
     * \tparam Functor The type of the functor provided.
     * \tparam Parameters The parameters the event provides when triggered.
     * \param event The event to subscribe the provided functor to.
     * \param functor The functor to subscribe to the provided event.
     */
    template<typename Functor, typename... Parameters>
    void addNonConstAutoHook(Event<Parameters...>& event, Functor&& functor) {
        auto delegate = event.template addNonConstFunctor<Functor>(std::forward<Functor>(functor));

        m_deletionFunctions.emplace_back(makeFunctor([delegate, &event] () {
            event -= *delegate;
        }));
    }

    /*! \brief Properly disposes all delegates added to this pool. */
    void dispose() {
        // Delete each bound functor.
        for (auto& deletor : m_deletionFunctions) {
            deletor.invoke();
        }

        // Clear the list of deletors.
        DeletorList().swap(m_deletionFunctions);
    }
private:
    DeletorList m_deletionFunctions; ///< List of delegates to be deleted
};

#endif // !Vorb_Event_h__
