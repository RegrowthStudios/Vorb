//
// Delegate.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Delegate.hpp
* @brief C#-style delegates.
*/

#pragma once

#ifndef Vorb_Delegate_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Delegate_h__
//! @endcond

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

#include <type_traits>
#include <utility>

/*
  There are three main ways that a delegate can be constructed and called.

  1. Static function
       Requires:
           - Function pointer
  2. Member function with object reference
       Requires:
           - Function pointer
           - Object reference
  3. Member function with object allocated and owned
       Requires:
           - Function pointer
           - Copied object pointer
           - Destructor

  A lambda or functor class can convert to one of these delegate call-types. For a static function conversion,
  the implicit conversion operator to a function pointer must be defined. The MSVC compiler generates the conversion
  operator for lambdas that have no state. Otherwise, one must pick whether to create allocations for the lambda, or
  have a reference delegate that is called while the lambda or functor is allocated.
*/

// Delegates may sometimes need to handle deallocation of their object instances.
// Instead of giving the delegate a function pointer to a destructor, we can save
// some memory per delegate by having a global list of destructors and delegates
// will get an index to that destructor.
namespace DelegateDestructor {
    // Type for the destructor function stub.
    typedef void(*Destructor)(void*);

    // Get a new destructor ID for a new stub.
    size_t genDestructorID(Destructor f);

    // Destructor template stub defined for a specific type.
    template<typename T>
    void destroyObjectFunc(void *pObject) {
        delete (T*)pObject;
    }

    // Get the index to the destructor for type T.
    template<typename T>
    size_t destructorID() {
        static size_t g_id = genDestructorID(destroyObjectFunc<T>);
        return g_id;
    }

    // Destroy an object with the specified destructor.
    void destroy(void* pObject, size_t id);
}

class DelegateBase {
protected:
    // Empty class definition for spoofing method calls.
    class EmptyClass { /* Empty */ };
public:
    typedef void* Caller;
    typedef const void* ConstCaller;
    typedef void* Function;
    typedef void (EmptyClass::*MemberFunction)();
    typedef void (EmptyClass::*MemberFunctionConst)() const;
    typedef void(*Deleter)(Caller);

    static_assert(sizeof(Deleter) == sizeof(ptrdiff_t), "Integral pointer conversion is flawed");

    DelegateBase() = default;

    // Delegate data can be moved and object ownership is transferred in this move operation.
    DelegateBase(DelegateBase&& other) :
        m_caller(other.m_caller),
        m_func(other.m_func),
        m_meta(other.m_meta) {
        other.m_caller = nullptr;
        other.m_func = nullptr;
        other.m_meta = 0;
    }
    DelegateBase& operator=(DelegateBase&& other) {
        if (m_flagRequiresDeletion != 0) {
            DelegateDestructor::destroy(m_caller, m_deleter);
        }
        m_caller = other.m_caller;
        other.m_caller = nullptr;
        m_func = other.m_func;
        other.m_func = nullptr;
        m_meta = other.m_meta;
        other.m_meta = 0;
        return *this;
    }

    // Delete copy operators so that copies aren't accidentally performed and memory is aggressively allocated or deallocated.
    // It would be possible to re-enable these, but at the cost of allocating object copies whenever m_flagRequiresDeletion is
    // true.
    DelegateBase(const DelegateBase&) = delete;
    DelegateBase& operator=(const DelegateBase&) = delete;

    ~DelegateBase() {
        if (m_flagRequiresDeletion != 0) {
            DelegateDestructor::destroy(m_caller, m_deleter);
        }
    }

    union // Pointer to an object instance for calling an object method.
    {
        Caller              m_caller;
        ConstCaller         m_callerConst;
        EmptyClass         *m_callerEmpty;
    };
    union // Pointer to a function/object method that the delegate will invoke.
    {
        Function            m_func;
        MemberFunction      m_memberFunc;
        MemberFunctionConst m_memberFuncConst;
    };
protected:
    union {
        size_t              m_meta;
        struct {
            size_t          m_deleter : (sizeof(size_t) * 8 - 2); // Identifier for the deleter to access when destroying the caller.
            size_t          m_flagIsObject : 1; // We use this to differentiate between object and normal calls because it's faster than an extra function indirection.
            size_t          m_flagRequiresDeletion : 1; // True if we need to invoke the deleter on the caller this delegate holds.
        };
    };
};

template<typename Ret, typename... Args>
class Delegate : public DelegateBase {
    template<template<typename> class TList, typename... Params> friend class Event;
public:
    typedef Ret(*TypedSimpleFunction)(Args...);

    template <typename T>
    using TypedMemberFunction = Ret(T::*)(Args...);

    template <typename T>
    using TypedMemberFunctionConst = Ret(T::*)(Args...) const;

    Delegate() : DelegateBase() {
        // Empty
    }

    // Delete copy operators for same reasons as DelegateBase.
    Delegate(const Delegate&) = delete;
    Delegate& operator=(const Delegate&) = delete;

    // Move constructors from base class
    Delegate(Delegate&& other) : DelegateBase(std::move(other)) {
        // Empty
    }
    Delegate& operator=(Delegate&& other) {
        return (Delegate&)DelegateBase::operator=(std::move(other));
    }

    // See del::create.
    static Delegate create(TypedSimpleFunction f) {
        Delegate value;
        value.m_func = f;
        value.m_caller = nullptr;
        value.m_meta = 0;
        return value;
    }

    // See del::create.
    template<typename T>
    static Delegate create(T* o, TypedMemberFunction<T> f) {
        Delegate value;
        value.m_memberFunc = (MemberFunction)f;
        value.m_caller = o;
        value.m_deleter = 0;
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = false;
        return value;
    }

    // See del::create.
    template<typename T>
    static Delegate create(const T* o, TypedMemberFunctionConst<T> f) {
        Delegate value;
        value.m_memberFuncConst = (MemberFunctionConst)f;
        value.m_callerConst = o;
        value.m_deleter = 0;
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = false;
        return value;
    }

    // See del::createWithAlloc.
    template<typename T>
    static Delegate createAllocAndCopyObject(const T* o, TypedMemberFunction<T> f) {
        Delegate value;
        value.m_memberFunc = (MemberFunction)f;
        value.m_caller = new T(*o); // Copy construct
        value.m_deleter = DelegateDestructor::destructorID<T>();
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = true;
        return value;
    }

    // See del::createWithAlloc.
    template<typename T>
    static Delegate createAllocAndCopyObject(const T* o, TypedMemberFunctionConst<T> f) {
        Delegate value;
        value.m_memberFuncConst = (MemberFunctionConst)f;
        value.m_caller = new T(*o); // Copy construct
        value.m_deleter = DelegateDestructor::destructorID<T>();
        value.m_flagIsObject = true;
        value.m_flagRequiresDeletion = true;
        return value;
    }

    // Correctly invoke the method this delegate references
    Ret invoke(Args... args) const {
        if (m_flagIsObject) {
            auto blankedFunction = (TypedMemberFunction<EmptyClass>)m_memberFunc;
            return (m_callerEmpty->*blankedFunction)(args...);
        }
        else {
            return ((TypedSimpleFunction)m_func)(args...);
        }
    }
    Ret operator()(Args... args) const {
        if (m_flagIsObject) {
            auto blankedFunction = (TypedMemberFunction<EmptyClass>)m_memberFunc;
            return (m_callerEmpty->*blankedFunction)(args...);
        }
        else {
            return ((TypedSimpleFunction)m_func)(args...);
        }
    }

    // Create a copy of this delegate. If this delegate has an allocation
    // associated with it, the weak copy cannot be invoked if this delegate
    // is changed (causing the allocation to be destroyed).
    Delegate weakCopy() const {
        Delegate v;
        v.m_caller = m_caller;
        v.m_func = m_func;
        v.m_meta = 0;
        v.m_flagIsObject = m_flagIsObject;
        return v;
    }

    // Equivalence is only based on object and method pointers.
    bool operator==(const Delegate& other) const {
        return m_memberFunc == other.m_memberFunc && m_caller == other.m_caller;
    }
    bool operator!=(const Delegate& other) const {
        return m_memberFunc != other.m_memberFunc || m_caller != other.m_caller;
    }
};

// Utility classes used to make sure that functors (lambdas) are properly converted into the right delegate types.
template <typename Functor, bool IsCaptureLess>
struct DelegateFunctor : public DelegateFunctor < decltype(&Functor::operator()), IsCaptureLess > { /* Empty */ };

template<typename Functor, typename Ret, typename... Args>
struct DelegateFunctor < Ret(Functor::*)(Args...) const, true > {
public:
    typedef Delegate<Ret, Args...> type;

    static type create(const Functor& obj) {
        // Access the conversion operator
        typename type::TypedSimpleFunction pSimpleFunc = (typename type::TypedSimpleFunction)obj;

        return type::create(pSimpleFunc);
    }
};

template<typename Functor, typename Ret, typename... Args>
struct DelegateFunctor < Ret(Functor::*)(Args...) const, false > {
public:
    typedef Delegate<Ret, Args...> type;

    static type create(const Functor& obj) {
        return type::create<Functor>(&obj, &Functor::operator());
    }
    static type createWithAlloc(const Functor& obj) {
        return type::createAllocAndCopyObject<Functor>(&obj, &Functor::operator());
    }
};

// Use a namespace so that we don't clutter the global namespace with identifiers that could be common.
namespace del {
    // Create a delegate from a simple function pointer.
    template<typename Ret, typename... Args>
    Delegate<Ret, Args...> create(Ret(*f)(Args...)) {
        return Delegate<Ret, Args...>::create(f);
    }

    // Create a delegate from an object and a method pointer. It is expected that the object remains
    // valid while this delegate is being used.
    template<typename Object, typename Ret, typename... Args>
    Delegate<Ret, Args...> create(Object& obj, Ret(Object::*f)(Args...)) {
        return Delegate<Ret, Args...>::create<Object>(&obj, f);
    }

    // Create a delegate from a maybe-const object and a const method pointer. It is expected that the 
    // object remains valid while this delegate is being used.
    template<typename Object, typename Ret, typename... Args>
    Delegate<Ret, Args...> create(const Object& obj, Ret(Object::*f)(Args...) const) {
        return Delegate<Ret, Args...>::create<Object>(&obj, f);
    }

    // Create a delegate from a functor that is captureless or has a user-defined conversion operator to a
    // simple function pointer. The delegate does not require any more reference from the functor object.
    template<typename Functor>
    typename DelegateFunctor<Functor, true>::type createFromFunctor(const Functor& obj) {
        return DelegateFunctor<Functor, true>::create(obj);
    }

    // Create a delegate from a functor, but the reference to the functor is required to persist while the 
    // delegate is being used.
    template<typename Functor>
    typename DelegateFunctor<Functor, true>::type createFromFunctorPersisting(const Functor& obj) {
        return DelegateFunctor<Functor, false>::create(obj);
    }

    // Create a delegate from an object and a method pointer. Allocate a copy of the object passed in, but 
    // the object passed in isn't required to be non-const. Object passed in won't be modified from invocations
    // of this delegate. The delegate does not require any more reference from the object passed in.
    template<typename Object, typename Ret, typename... Args>
    Delegate<Ret, Args...> createWithAlloc(const Object& obj, Ret(Object::*f)(Args...)) {
        return Delegate<Ret, Args...>::createAllocAndCopyObject<Object>(&obj, f);
    }

    // Create a delegate from an object and a const method pointer. Allocate a copy of the object passed in.
    // The delegate does not require any more reference from the object passed in.
    template<typename Object, typename Ret, typename... Args>
    Delegate<Ret, Args...> createWithAlloc(const Object& obj, Ret(Object::*f)(Args...) const) {
        return Delegate<Ret, Args...>::createAllocAndCopyObject<Object>(&obj, f);
    }

    // Create a delegate from a functor that is passed in and allocate a replica of that functor, regardless of
    // whether it can be converted to a simple function pointer or not. The delegate does not require any more reference from the functor object.
    template<typename Functor>
    typename DelegateFunctor<Functor, false>::type createWithAllocFromFunctor(const Functor& obj) {
        return DelegateFunctor<Functor, false>::createWithAlloc(obj);
    }
}

#endif // !Vorb_Delegate_hpp__
