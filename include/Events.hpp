//
// Events.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/************************************************************************/
/*                                                                      */
/* Reader beware, this is not for the faint of heart.                   */
/*                                                                      */
/*      ... but for those brave adventurers, who delve into the         */
/*          deep abysses ...                                            */
/*                                                                      */
/*                          ... once you have discovered how            */
/*                              terrifying is the monstrosity           */
/*                              below, mark your name here.             */
/*                                                                      */
/*   So that future readers                                             */
/*           near and far                                               */
/*                     will understand your agony                       */
/*                              and mail you a cherry pie               */
/*                                                                      */
/*  Ben Arnold                                                          */
/*                                                                      */
/************************************************************************/

/*! \file Events.hpp
 * @brief C#-style events.
 */

#pragma once

#ifndef Vorb_Events_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Events_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include <vector>
#endif // !VORB_USING_PCH

#include "TypeManip.hpp"

/*! @brief Calling conventions 
 * 
 * 1. Static function
 *      Requires:
 *          - Function pointer
 * 2. Reference member function
 *      Requires:
 *          - Function pointer
 *          - Object reference
 * 3. Copied member function
 *      Requires:
 *          - Function pointer
 *          - Copied object pointer
 *          - Destructor
 * 4. Zero-state functor object (converts to function pointer, some lambdas)
 *      Requires:
 *          - Function pointer
 *          - Object reference
 * 5. Captured-state reference functor object
 *      Requires:
 *          - Function pointer
 *          - Functor object
 * 6. Captured-state copied functor object
 *      Requires:
 *          - Function pointer
 *          - Copied functor object pointer
 *          - Destructor
 */

typedef const void* Sender; ///< A pointer to an object that sent the event
template<typename... Params> class Event;

class DelegateBase {
public:
    typedef void* Caller;
    typedef void* Function;
	typedef void (TypelessMember::*MemberFunction)();
    typedef void(*Deleter)(Caller);
    typedef void* UnknownStub;

    static_assert(sizeof(Deleter) == sizeof(ptrdiff_t), "Integral pointer conversion is flawed");
	
    ~DelegateBase() {
        if (m_flagRequiresDeletion != 0) {
            ((Deleter)m_deleters[m_deleter])(m_caller);
        }
    }

    Caller m_caller;
	union {
		Function m_func;
		MemberFunction m_memberFunc;
	};
protected:
    static std::vector<Deleter> m_deleters;
    static size_t grabDeleter(Deleter f) {
        size_t i = m_deleters.size();
        m_deleters.push_back(f);
        return i;
    }
    template<typename T>
    static size_t deleterID() {
        static size_t id = grabDeleter(freeObject<T>);
        return id;
    }

    template<typename T>
    static void freeObject(Caller c) {
        delete (T*)c;
    }

    void neuter() {
        m_deleter = 0;
    }
    template<typename T>
    void engorge() {
        m_deleter = deleterID<T>;
    }

    struct {
        size_t m_deleter : (sizeof(size_t) * 8 - 2);
        size_t m_flagIsObject : 1;
        size_t m_flagRequiresDeletion : 1;
    };
};

template<typename Ret, typename... Args>
class RDelegate : public DelegateBase {
    template<typename... Params> friend class Event;
public:
    typedef Ret(*CallStub)(Caller, Function, Args...); ///< Function type for a stub

    static RDelegate create(Ret(*f)(Args...)) {
        RDelegate value = {};
        value.m_func = f;
        value.m_caller = nullptr;
        value.m_deleter = 0;
        value.m_flagIsObject = false;
        value.m_flagRequiresDeletion = false;
        return value;
    }
    template<typename T>
    static RDelegate create(const T* o, Ret(T::*f)(Args...) const) {
        RDelegate value = {};
		value.m_memberFunc = (MemberFunction)f;
		value.m_caller = const_cast<T*>(o);
        value.m_deleter = 0;
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = false;
        return value;
    }
    template<typename T>
    static RDelegate* createCopy(const T* o, Ret(T::*f)(Args...) const) {
        RDelegate* value = new RDelegate();
		value->m_memberFunc = (MemberFunction)f;
		value->m_caller = new T(*const_cast<T*>(o));
        value->m_deleter = deleterID<T>();
        value->m_flagIsObject = true;
        value->m_flagRequiresDeletion = true;
        return value;
    }
    template<typename T>
    static RDelegate create(T* o, Ret(T::*f)(Args...)) {
        RDelegate value = {};
        value.m_memberFunc = (MemberFunction)f;
        value.m_caller = o;
        value.m_deleter = 0;
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = false;
        return value;
    }
    template<typename T>
    static RDelegate* createCopy(T* o, Ret(T::*f)(Args...)) {
        RDelegate* value = new RDelegate();
        value->m_memberFunc = (MemberFunction)f;
        value->m_caller = new T(*o);
        value->m_deleter = deleterID<T>();
        value->m_flagIsObject = true;
        value->m_flagRequiresDeletion = true;
        return value;
    }

    Ret invoke(Args... args) const {
        if (m_flagIsObject == 0) {
            return ((Ret(*)(Args...))m_func)(args...);
        } else {
			auto blankedFunction = (Ret(TypelessMember::*)(Args...))m_memberFunc;
			return (((TypelessMember*)m_caller)->*blankedFunction)(args...);
        }
    }
    Ret operator()(Args... args) const {
        if (m_flagIsObject == 0) {
            return ((Ret(*)(Args...))m_func)(args...);
        } else {
			auto blankedFunction = (Ret(TypelessMember::*)(Args...))m_memberFunc;
			return (((TypelessMember*)m_caller)->*blankedFunction)(args...);
        }
    }

    bool operator==(const RDelegate& other) const {
        return m_memberFunc == other.m_memberFunc && m_caller == other.m_caller;
    }
    bool operator!=(const RDelegate& other) const {
        return m_memberFunc != other.m_memberFunc || m_caller != other.m_caller;
    }
protected:
    template<typename T>
    static Ret objectCall(Caller obj, Function func, Args... args) {
        typedef Ret(T::*fType)(Args...);
        
        // Properly cast internal values
        auto p = static_cast<T*>(obj);
        fType f = *(fType*)&func;

        // Call function using object
        return (p->*f)(args...);
    }
    static Ret simpleCall(Caller obj, Function func, Args... args) {
        typedef Ret(*fType)(Args...);

        // Call function
        return ((fType)func)(args...);
    }
};

template<typename... Args>
class Delegate : public RDelegate<void, Args...> {
public:
    static Delegate create(void(*f)(Args...)) {
        Delegate value = {};
        value.m_func = f;
        value.m_caller = nullptr;
        value.m_deleter = 0;
        return value;
    }
    template<typename T>
    static Delegate create(const T* o, void(T::*f)(Args...) const) {
        Delegate value = {};
        value.m_func = *(Function*)&f;
        value.m_caller = const_cast<T*>(o);
        value.m_deleter = 0;
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = false;
        return value;
    }
    template<typename T>
    static Delegate* createCopy(const T* o, void(T::*f)(Args...) const) {
        Delegate* value = new Delegate();
        value->m_func = *(Function*)&f;
        value->m_caller = new T(*const_cast<T*>(o));
        value->m_deleter = deleterID<T>();
        value->m_flagIsObject = true;
        value->m_flagRequiresDeletion = true;
        return value;
    }
    template<typename T>
    static Delegate create(T* o, void(T::*f)(Args...)) {
        Delegate value = {};
        value.m_func = *(Function*)&f;
        value.m_caller = o;
        value.m_deleter = 0;
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = false;
        return value;
    }
    template<typename T>
    static Delegate* createCopy(T* o, void(T::*f)(Args...)) {
        Delegate* value = new Delegate();
        value->m_func = *(Function*)&f;
        value->m_caller = new T(*o);
        value->m_deleter = deleterID<T>();
        value->m_flagIsObject = true;
        value->m_flagRequiresDeletion = true;
        return value;
    }
};

template <typename T>
struct DelegateType : public DelegateType < decltype(&T::operator()) > {};

template<typename F, typename Ret, typename... Args>
struct DelegateType < Ret(F::*)(Args...) const > {
public:
    typedef Delegate<Args...> type;
};

template <typename T>
struct RDelegateType : public RDelegateType < decltype(&T::operator()) > {};

template<typename F, typename Ret, typename... Args>
struct RDelegateType < Ret(F::*)(Args...) const > {
public:
    typedef RDelegate<Ret, Args...> type;
};

template<typename Ret, typename... Args>
RDelegate<Ret, Args...> makeRDelegate(Ret(*f)(Args...)) {
    return RDelegate<Ret, Args...>::create(f);
}
template<typename T, typename Ret, typename... Args>
RDelegate<Ret, Args...> makeRDelegate(T& obj, Ret(T::*f)(Args...)const) {
    return RDelegate<Ret, Args...>::create<T>(&obj, f);
}
template<typename T, typename Ret, typename... Args>
RDelegate<Ret, Args...> makeRDelegate(T& obj, Ret(T::*f)(Args...)) {
    return RDelegate<Ret, Args...>::create<T>(&obj, f);
}
template<typename Ret, typename... Args, typename F>
RDelegate<Ret, Args...> makeRDelegate(F& obj) {
    typedef Ret(F::*fType)(Args...) const;
    fType f = &F::operator();
    return RDelegate<Ret, Args...>::create<F>(&obj, f);
}

template<typename F>
typename RDelegateType<F>::type* makeRFunctor(F& obj) {
    return RDelegateType<F>::type::createCopy<F>(&obj, &F::operator());
}
template<typename T, typename Ret, typename... Args>
RDelegate<Ret, Args...>* makeRFunctor(T& obj, Ret(T::*f)(Args...)const) {
    return RDelegate<Ret, Args...>::createCopy<T>(&obj, f);
}
template<typename T, typename Ret, typename... Args>
RDelegate<Ret, Args...>* makeRFunctor(T& obj, Ret(T::*f)(Args...)) {
    return RDelegate<Ret, Args...>::createCopy<T>(&obj, f);
}

template<typename... Args>
Delegate<Args...> makeDelegate(void(*f)(Args...)) {
    return Delegate<Args...>::create(f);
}
template<typename T, typename... Args>
Delegate<Args...> makeDelegate(T& obj, void(T::*f)(Args...)const) {
    return Delegate<Args...>::create<T>(&obj, f);
}
template<typename T, typename... Args>
Delegate<Args...> makeDelegate(T& obj, void(T::*f)(Args...)) {
    return Delegate<Args...>::create<T>(&obj, f);
}
template<typename... Args, typename F>
Delegate<Args...> makeDelegate(F& obj) {
    typedef void(F::*fType)(Args...) const;
    fType f = &F::operator();
    return Delegate<Args...>::create<F>(&obj, f);
}

template<typename F>
typename DelegateType<F>::type* makeFunctor(F& obj) {
    return DelegateType<F>::type::createCopy<F>(&obj, &F::operator());
}
template<typename T, typename... Args>
Delegate<Args...>* makeFunctor(T& obj, void(T::*f)(Args...)const) {
    return Delegate<Args...>::createCopy<T>(&obj, f);
}
template<typename T, typename... Args>
Delegate<Args...>* makeFunctor(T& obj, void(T::*f)(Args...)) {
    return Delegate<Args...>::createCopy<T>(&obj, f);
}

/// An event that invokes methods taking certain arguments
/// @tparam Params: Metadata sent in event invocation
template<typename... Params>
class Event {
public:
    typedef RDelegate<void, Sender, Params...> Listener; ///< Callback delegate type

    /// Create an event with a sender attached to it
    /// @param sender: Owner object sent with each invocation
    Event(Sender sender = nullptr) :
        m_sender(sender) {
        // Empty
    }

    /*! @brief Reset the sender value of this event to another.
     * 
     * @warning Resetting a sender of an event could cause harm to existing listeners.
     * 
     * @param s: New sender pointer.
     */
    void setSender(Sender s) {
        m_sender = s;
    }

    /// Call all bound methods
    /// @param p: Arguments used in function calls
    void send(Params... p) {
        size_t sz = m_funcs.size();
        for (size_t i = 0; i < sz && i < m_funcs.size(); i++) {
            m_funcs[i](m_sender, p...);
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
    Event& add(Listener f) {
        f.neuter();
        m_funcs.push_back(f);
        return *this;
    }
    /// Add a function to this event
    /// @param f: A unknown-type subscriber
    /// @tparam F: Functor type
    /// @return The newly made delegate (CALLER DELETE)
    template<typename F>
    Listener* addFunctor(F f) {
        Listener* functor = makeRFunctor(f);
        this->add(*functor);
        return functor;
    }

    /// Add a function to this event whilst allowing chaining
    /// @param f: A subscriber
    /// @return Self
    Event& operator +=(const Listener& f) {
        return add(f);
    }

    /// Remove a function (just one) from this event
    /// @param f: A subscriber
    Event& remove(const Listener& f) {
        auto fLoc = std::find(m_funcs.begin(), m_funcs.end(), f);
        if (fLoc == m_funcs.end()) return *this;
        m_funcs.erase(fLoc);
        return *this;
    }
    /// Remove a function (just one) from this event whilst allowing chaining
    /// @param f: A subscriber
    /// @return Self
    Event& operator -=(const Listener& f) {
        return remove(f);
    }
private:
    Sender m_sender; ///< Event owner
    std::vector<Listener> m_funcs; ///< List of bound functions (subscribers)
};

/// Manages destruction of generated delegates
class AutoDelegatePool {
public:
    typedef Delegate<>* Deleter; ///< A deletion function prototype
    typedef std::vector<Deleter> DeleterList; ///< A container of deleters

    /// Calls dispose
    virtual ~AutoDelegatePool() {
        dispose();
    }

    /// Binds a callback to an event and adds it for automatic destruction
    /// @tparam F: f's functor type
    /// @tparam Params: f's extra invocation parameters
    /// @param e: Event
    /// @param f: Callback function
    template<typename F, typename... Params>
    void addAutoHook(Event<Params...>& e, F f) {
        auto fd = e.addFunctor<F>(f);

        Deleter d = makeFunctor<>([fd, &e] () {
            e -= *fd;
            delete fd;
        });
        m_deletionFunctions.push_back(d);
    }

    /// Properly disposes all delegates added to this pool
    void dispose() {
        for (auto f : m_deletionFunctions) {
            f->invoke();
            delete f;
        }
        DeleterList().swap(m_deletionFunctions);
    }
private:
    DeleterList m_deletionFunctions; ///< List of delegates to be deleted
};

#endif // !Vorb_Events_hpp__
