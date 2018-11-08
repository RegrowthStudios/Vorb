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
    using Subscriber  = Delegate<void, Sender, Parameters...>;
    using Subscribers = std::vector<Subscriber>;
public:
    /*!
     * \brief Default Constructor
     *
     * \param sender The owner of the event.
     */
    Event(Sender sender = nullptr) :
        EventBase(sender)
    { /* Empty */ }
    Event(const Event& event) = delete;
    Event(Event&& event)      = delete;

    /*!
     * \brief Adds a subscriber to the event.
     *
     * \param subscriber The subscriber to add to the event.
     */
    void add(Subscriber subscriber) {
        // We don't want this copy of the delegate to manage any internal object that may be present.
        subscriber.neuter();

        m_subscribers.emplace_back(subscriber);
    }
    /*!
     * \brief Adds a subscriber to the event.
     *
     * \param subscriber The subscriber to add to the event.
     */
    void add(Subscriber&& subscriber) {
        m_subscribers.emplace_back(subscriber);
    }
    /*!
     * \brief Adds a subscriber to the event.
     *
     * operator+= is an alias of add
     *
     * \param subscriber The subscriber to add to the event.
     */
    void operator+=(const Subscriber& subscriber) {
        add(subscriber);
    }
    /*!
     * \brief Adds a subscriber to the event.
     *
     * operator+= is an alias of add
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
    CALLER_DELETE Subscriber* addFunctor(Functor functor) {
        Subscriber* subscriber = makeFunctor(functor);
        
        add(*subscriber);

        return subscriber;
    }

    /*!
     * \brief Removes a subscriber from the event (only the first instance found is removed).
     *
     * \param subscriber The subscriber to remove from the event.
     */
    void remove(const Subscriber& subscriber) {
        const auto& it = std::find(m_subscribers.begin(), m_subscribers.end(), subscriber);
        if (it != m_subscribers.end()) {
            m_subscribers.erase(it);
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
        for (auto& subscriber : m_subscribers) {
            subscriber(m_sender, parameters...);
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
        trigger(std::forward<Parameters...>(parameters...));
    }
protected:
    Subscribers m_subscribers;
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
    void addAutoHook(Event<Parameters...>& event, Functor functor) {
        auto delegate = event.template addFunctor<Functor>(functor);

        Deletor deletor = makeFunctor([&delegate, &event] () {
            event -= *delegate;
            delete delegate;
        });

        m_deletionFunctions.push_back(deletor);
    }

    /*! \brief Properly disposes all delegates added to this pool. */
    void dispose() {
        // Delete each bound functor.
        for (auto deletor : m_deletionFunctions) {
            deletor->invoke();
            delete deletor;
        }

        // Clear the list of deletors.
        DeletorList().swap(m_deletionFunctions);
    }
private:
    DeletorList m_deletionFunctions; ///< List of delegates to be deleted
};

#endif // !Vorb_Event_h__
