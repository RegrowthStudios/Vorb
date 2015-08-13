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

template<typename T>
union PointerCast {
    static_assert(sizeof(T) == sizeof(void*), "Value is not same size as a pointer");

    PointerCast(T v) {
        value = v;
    }
    PointerCast(void* v) {
        ptr = v;
    }

    T value;
    void* ptr;
};

class BlankTemplate {
    template<typename Ret, typename... Params>
    Ret function(Params... args);
};

template<size_t A1, size_t BITS>
struct MinEvenAlignment {
public:
    static const size_t bitmask = (1 << BITS) - 1;
    static const size_t value = (A1 & bitmask) ? ((A1 | bitmask) + 1) : A1;
};

template<typename T, size_t BITS>
void* copyObjectAligned(T& reference) {
    void* storage = new std::aligned_storage<sizeof(T), MinEvenAlignment<std::alignment_of<T>::value, BITS>::value>();
    new (storage)T(reference);
    return storage;
}
template<typename T, size_t BITS>
void* copyObjectAligned(T&& reference) {
    void* storage = new std::aligned_storage<sizeof(T), MinEvenAlignment<std::alignment_of<T>::value, BITS>::value>();
    new (storage)T(std::move(reference));
    return storage;
}

class DelegateBase {
public:
    typedef void* Caller;
    typedef void* Function;
    typedef void(*Deleter)(Caller);
    typedef void* UnknownStub;

    static_assert(sizeof(Caller) == sizeof(ptrdiff_t), "Integral pointer conversion is flawed");

    DelegateBase(Caller c, Function f, UnknownStub s, Deleter d) :
        m_caller(c),
        m_func(f),
        m_stub(s),
        m_deletion(d) {
        // Empty
    }
    DelegateBase() :
        m_caller(nullptr),
        m_func(nullptr),
        m_stub(nullptr),
        m_deletion(doNothing) {
        // Empty
    }
    ~DelegateBase() {
        m_deletion(m_caller);
    }

    union {
        Caller m_caller;
        struct {
            ptrdiff_t m_callerHigh : (sizeof(Caller) * 8 - 1);
            ptrdiff_t m_callerFlag1 : 1;
            ptrdiff_t m_callerFlag2 : 1;
        };
    };
    Function m_func;
protected:
    static void doNothing(Caller c) {
        // Empty
    }
    template<typename T>
    static void freeObject(Caller c) {
        T* obj = (T*)c;
        obj->~T();
        operator delete(c);
    }

    void neuter() {
        m_deletion = doNothing;
    }
    template<typename T>
    void engorge() {
        m_deletion = freeObject<T>;
    }

    Deleter m_deletion;
    UnknownStub m_stub;
};

template<typename Ret, typename... Args>
class RDelegate : public DelegateBase {
    template<typename... Params> friend class Event;
public:
    typedef Ret(*CallStub)(Caller, Function, Args...); ///< Function type for a stub

    RDelegate(Caller c, Function f, CallStub s, Deleter d) : DelegateBase(c, f, s, d) {
        // Empty
    }
    RDelegate() : DelegateBase(nullptr, nullptr, simpleCall, doNothing) {
        // Empty
    }

    template<typename T>
    static RDelegate create(const T* o, Ret(T::*f)(Args...) const) {
        return RDelegate(const_cast<T*>(o), *(Function*)&f, objectCall<T>, doNothing);
    }
    template<typename T>
    static RDelegate* createCopy(const T* o, Ret(T::*f)(Args...) const) {
        T* no = (T*)operator new(sizeof(T));
        new (no)T(*o);
        return new RDelegate(no, *(Function*)&f, objectCall<T>, freeObject<T>);
    }
    template<typename T>
    static RDelegate create(T* o, Ret(T::*f)(Args...)) {
        return RDelegate(o, *(Function*)&f, objectCall<T>, doNothing);
    }
    template<typename T>
    static RDelegate* createCopy(T* o, Ret(T::*f)(Args...)) {
        T* no = (T*)operator new(sizeof(T));
        new (no) T(*o);
        return new RDelegate(no, *(Function*)&f, objectCall<T>, freeObject<T>);
    }
    static RDelegate create(Ret(*f)(Args...)) {
        return RDelegate(nullptr, f, simpleCall, doNothing);
    }

    Ret invoke(Args... args) const {
        return ((CallStub)m_stub)(m_caller, m_func, args...);
    }
    Ret operator()(Args... args) const {
        return ((CallStub)m_stub)(m_caller, m_func, args...);
    }

    bool operator==(const RDelegate& other) const {
        return m_func == other.m_func && m_caller == other.m_caller;
    }
    bool operator!=(const RDelegate& other) const {
        return m_func != other.m_func || m_caller != other.m_caller;
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
    Delegate(Caller c, Function f, CallStub s, Deleter d) : RDelegate<void, Args...>(c, f, s, d) {
        // Empty
    }
    Delegate() : RDelegate<void, Args...>() {
        // Empty
    }

    template<typename T>
    static Delegate create(const T* o, void(T::*f)(Args...) const) {
        return Delegate(const_cast<T*>(o), *(Function*)&f, objectCall<T>, doNothing);
    }
    template<typename T>
    static Delegate* createCopy(const T* o, void(T::*f)(Args...) const) {
        T* no = (T*)operator new(sizeof(T));
        new (no)T(*o);
        return new Delegate(no, *(Function*)&f, objectCall<T>, freeObject<T>);
    }
    template<typename T>
    static Delegate create(T* o, void(T::*f)(Args...)) {
        return Delegate(o, *(Function*)&f, objectCall<T>, doNothing);
    }
    template<typename T>
    static Delegate* createCopy(T* o, void(T::*f)(Args...)) {
        T* no = (T*)operator new(sizeof(T));
        new (no)T(*o);
        return new Delegate(no, *(Function*)&f, objectCall<T>, freeObject<T>);
    }
    static Delegate create(void(*f)(Args...)) {
        return Delegate(nullptr, f, simpleCall, doNothing);
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
