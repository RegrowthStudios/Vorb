///
/// Events.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 8 Nov 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// C#-style events
///

#pragma once

#ifndef Events_h__
#define Events_h__

/// This is the main function pointer
/// @tparam Params: Additional metadata used in delegate invocation
template<typename... Params>
class IDelegate {
public:
    /// Invoke this function's code
    /// @param sender: The sender that underlies the event
    /// @param p: Additional arguments to function
    virtual void invoke(void* sender, Params... p) = 0;
};

/// Functor object to hold instances of anonymous lambdas
/// @tparam F: Auto-generated lambda function type
/// @tparam Params: Additional metadata used in delegate invocation
template<typename F, typename... Params>
class Delegate : public IDelegate<Params...> {
public:
    /// Empty constructor
    Delegate() {
        // Empty
    }
    /// Copy constructor
    /// @param f: Lambda functor
    Delegate(F& f) :
        m_f(f) {
        // Empty
    }

    /// Invoke this functor's code
    /// @param sender: The sender that underlies the event
    /// @param p: Additional arguments to function
    virtual void invoke(void* sender, Params... p) override {
        m_f(sender, p...);
    }
private:
    F m_f; ///< Type-inferenced lambda functor
};

/// Use the compiler to generate a delegate
/// @param f: Lambda functor
/// @tparam F: Auto-generated lambda function type
/// @tparam Params: Additional metadata used in delegate invocation
/// @return Pointer to delegate created on the heap (CALLER DELETE)
template<typename... Params, typename F>
inline IDelegate<Params...>* createDelegate(F f) {
    return new Delegate<F, Params...>(f);
}

/// An event that invokes methods taking certain arguments
/// @tparam Params: Metadata sent in event invocation
template<typename... Params>
class Event {
public:
    typedef IDelegate<Params...>* Listener; ///< Callback delegate type

    /// Create an event with a sender attached to it
    /// @param sender: Owner object sent with each invokation
    Event(void* sender = nullptr) :
        m_sender(sender) {
        // Empty
    }

    /// Call all bound methods
    /// @param p: Arguments used in function calls
    void send(Params... p) {
        for (auto& f : m_funcs) {
            f->invoke(m_sender, p...);
        }
    }
    /// Call all bound methods
    /// @param p: Arguments used in function calls
    void operator()(Params... p) {
        this->send(p...);
    }

    /// Add a function to this event
    /// @param f: A subscriber
    /// @return The delegate passed in
    Listener add(Listener f) {
        if (f == nullptr) return nullptr;
        m_funcs.push_back(f);
        return f;
    }
    /// Add a function to this event
    /// @param f: A unknown-type subscriber
    /// @tparam F: Functor type
    /// @return The newly made delegate (CALLER DELETE)
    template<typename F>
    Listener addFunctor(F f) {
        Listener d = new Delegate<F, Params...>(f);
        return this->add(d);
    }
    /// Add a function to this event whilst allowing chaining
    /// @param f: A subscriber
    /// @return Self
    Event& operator +=(Listener f) {
        add(f);
        return *this;
    }
    
    /// Remove a function (just one) from this event
    /// @param f: A subscriber
    void remove(Listener f) {
        if (f == nullptr) return;
        auto fLoc = std::find(m_funcs.begin(), m_funcs.end(), f);
        if (fLoc == m_funcs.end()) return;
        m_funcs.erase(fLoc);
    }
    /// Remove a function (just one) from this event whilst allowing chaining
    /// @param f: A subscriber
    /// @return Self
    Event& operator -=(Listener f) {
        remove(f);
        return *this;
    }
private:
    void* m_sender; ///< Event owner
    std::vector<Listener> m_funcs; ///< List of bound functions (subscribers)
};

#endif // Events_h__