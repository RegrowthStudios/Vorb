//
// Delegate.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 7 Nov 2018
// Based on the original implementation by Cristian Zaloj
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file Delegate.hpp
 * \brief Provides a definition of delegates, which wrap various callables in C++ that have the
 * same return and parameter signature. These can thus be stored in the same container with ease
 * and provide a common call method.
 */

#pragma once

#ifndef Vorb_Delegate_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Delegate_h__
//! @endcond

#include <cstdio>
#include "Vorb/types.h"

/****************************************************************************\
 *                             Brief Overview                               *
 *                             ¯¯¯¯¯¯¯¯¯¯¯¯¯¯                               *
 * The Delegate class provides a way to wrap various callables in a type    *
 * whose kind is only dependent on the ReturnType and Parameters of the     *
 * functions it stores. As such, all callables with the same signature      *
 * ignoring CV qualifiers can be stored in the same collections.            *
 *                                                                          *
 * We provide a set of creation routines:                                   *
 *                                                                          *
 *     make[Const|NonConst]Delegate:                                        *
 *         - Creates a delegate without copying the provided object (if an  *
 *           object is provided). In the case of member functions, the      *
 *           provided object must still exist when the delegate is called.  *
 *     make[Const|NonConst]Functor:                                         *
 *         - Creates a delegate, copying the provided object. We call       *
 *           the member function provided through the copied object, which  *
 *           is itself managed by the Delegate class. The provided  object  *
 *           thus need not exist at the calltime of the delegate.           *
 *                                                                          *
 * In both cases, if Const or NonConst is provided, we specifically look    *
 * for a function that is const or non-const respecitively. This solves a   *
 * problem with automatic function selection when overloads of both         *
 * constness are provided with the same signature.                          *
 *                                                                          *
 * With these functions we are able to create Delegates of the following    *
 * kinds of callable:                                                       *
 *                                                                          *
 *     Static Functions:                                                    *
 *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                                    *
 *         - Call makeDelegate, providing a pointer to the function.        *
 *                                                                          *
 *     Member Functions (Object Reference):                                 *
 *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                 *
 *         - Call make[Const|NonConst]Delegate, providing the object to     *
 *           reference and a pointer to the function.                       *
 *                                                                          *
 *     Member Functions (Object Copy):                                      *
 *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                      *
 *         - Call make[Const|NonConst]Functor, providing the object to      *
 *           copy and a pointer to the function.                            *
 *                                                                          *
 *     Lambdas (both mutable and not):                                      *
 *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                      *
 *         - Call makeFunctor, providing the lambda.                        *
 *                                                                          *
 *     Functor Classes (Object Reference):                                  *
 *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                  *
 *         - Call make[Const|NonConst]Delegate, providing the object to     *
 *           reference.                                                     *
 *                                                                          *
 *     Functor Classes (Object Copy):                                       *
 *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                       *
 *         - Call make[Const|NonConst]Functor, providing the object to      *
 *           copy.                                                          *
 *                                                                          *
 * Note that Functor Classes are any type which satisfies std::is_class and *
 * provides at least one override of the () operator.                       *
\****************************************************************************/

#include "TypeManip.hpp"

// If we are compiling with old MSVC toolsets, then we can't use noexcept, so dummy it.
// We also have to deal with different syntaxes for function attributes between compilers.
#if !defined(_MSC_FULL_VER) || (_MSC_FULL_VER > 189999999)
#   define NOEXCEPT noexcept
#   if defined(__GNUG__)
#       define NORETURN __attribute__ ((noreturn))
#   else
#       define NORETURN
#   endif
#else
#   define NOEXCEPT
#   define NORETURN __declspec(noreturn)
#endif

#if !defined(REFCOUNT_DELEGATES)
#    define REFCOUNT_DELEGATES 0
#endif

#if !defined(PRINT_REFCOUNT_DELEGATES)
#    if defined(DEBUG)
#        define PRINT_REFCOUNT_DELEGATES 1
#    else
#        define PRINT_REFCOUNT_DELEGATES 0
#    endif
#endif

/****************************************************************************\
 *                     The Delegate Implementation                          *
\****************************************************************************/

// Empty base class of delegates to allow for non-templated polymorphic containers.
class DelegateBase {
    // Empty
};
/*!
 * \brief Delegate implementation, wraps callables of any kind with the given return type and parameter types. Ownership of objects is possible for functors.
 *
 * Note: Define the macro REFCOUNT_DELEGATES to 1 in order to have ref counted delegates. Further, for debug purposes you can define PRINT_REFCOUNT_DELEGATES to 1,
 * this defualts to 1 in the case DEBUG is defined.
 */
template<typename ReturnType, typename ...Parameters>
class Delegate : public DelegateBase {
protected:
    // Alias a whole bunch of types to make things more readable.
    using GenericObject         = const void*;
    using GenericFunction       = void(*)();
    using GenericMemberFunction = void(TypelessMember::*)();
    using Executor              = ReturnType(*)(GenericObject, GenericMemberFunction, Parameters...);
#if REFCOUNT_DELEGATES == 1
    using RefCount = ui16;
    using DeletorFunc           = void(*)(GenericObject);
    struct DeletorStruct {
        DeletorFunc del      = nullptr;
        RefCount    refCount = 0;
    };
    using Deletor               = DeletorStruct*;
#else
    using Deletor               = void(*)(GenericObject);
#endif // REFCOUNT_DELEGATES == 1
    using StaticFunction        = ReturnType(*)(Parameters...);
    template<typename SpecificClass>
    using MemberFunction        = ReturnType(SpecificClass::*)(Parameters...);
    template<typename SpecificClass>
    using ConstMemberFunction   = ReturnType(SpecificClass::*)(Parameters...) const;
public:
    // Constructors.
    Delegate() :
        m_object(nullptr), m_function(nullptr), m_executor(nullptr)
    { /* Empty */ }
    Delegate(GenericFunction function, Executor executor) :
        m_object(nullptr), m_function(function), m_executor(executor), m_deletor(nullptr)
    { /* Empty */ }
    Delegate(GenericObject object, GenericMemberFunction function, Executor executor, Deletor deletor = nullptr) :
        m_object(object), m_memberFunction(function), m_executor(executor), m_deletor(deletor)
#if REFCOUNT_DELEGATES == 1
    {
        if (deletor) {
            ++(deletor->refCount);
        }
    }
#else
    { /* Empty */ }
#endif // REFCOUNT_DELEGATES == 1
    /*!
     * WARNING: The object pointer is directly copied, rather than a deep copy of the object itself. Neuter this new instance or activate
     * ref counting if you don't want it prematurely deleting the object.
     */
    Delegate(const Delegate& delegate) :
        m_object(delegate.m_object), m_executor(delegate.m_executor), m_deletor(delegate.m_deletor) {
        // Make sure we assign the function pointer using the right type.
        if (m_object) {
            m_memberFunction = delegate.m_memberFunction;
#if REFCOUNT_DELEGATES == 1
            if (m_deletor) {
                ++m_deletor->refCount;
#   if PRINT_REFCOUNT_DELEGATES == 1
                // TODO(Matthew): Member function pointers can't be accurately printed through a cast to void*.
                printf("There are now %hu delegates with object: %p, and function pointer: %p.\n", m_deletor->refCount, (void*)m_object, (void*)m_function);
#   endif // PRINT_REFCOUNT_DELEGATES == 1
            }
#endif // REFCOUNT_DELEGATES == 1
        } else {
            m_function       = delegate.m_function;
        }
    }
    Delegate(Delegate&& delegate) NOEXCEPT :
        m_object(delegate.m_object), m_function(delegate.m_function), m_executor(delegate.m_executor), m_deletor(delegate.m_deletor) {
        // Make sure we assign the function pointer using the right type.
        if (m_object) {
            m_memberFunction = delegate.m_memberFunction;
        } else {
            m_function = delegate.m_function;
        }
        delegate.m_deletor = nullptr;
        delegate.m_object  = nullptr;
    }
    // Destructor.
    ~Delegate() {
        if (m_deletor) {
#if REFCOUNT_DELEGATES == 1
            if (m_deletor->refCount > 1) {
                --m_deletor->refCount;
#   if PRINT_REFCOUNT_DELEGATES == 1
                // TODO(Matthew): Member function pointers can't be accurately printed through a cast to void*.
                printf("There are now %hu delegates with object: %p, and function pointer: %p.\n", m_deletor->refCount, (void*)m_object, (void*)m_function);
#   endif // PRINT_REFCOUNT_DELEGATES == 1
            } else {
#   if PRINT_REFCOUNT_DELEGATES == 1
                // TODO(Matthew): Member function pointers can't be accurately printed through a cast to void*.
                printf("Deleting delegate with object: %p, and function pointer: %p.\n", (void*)m_object, (void*)m_function);
#   endif // PRINT_REFCOUNT_DELEGATES == 1
                m_deletor->del(m_object);
                delete m_deletor;
            }
#else
            m_deletor(m_object);
#endif // REFCOUNT_DELEGATES == 1
        }
        m_object   = nullptr;
        m_function = nullptr;
        m_executor = nullptr;
        m_deletor  = nullptr;
    }

    void neuter() {
#if REFCOUNT_DELEGATES == 1
        if (m_deletor) {
            --m_deletor->refCount;
        }
#endif // REFCOUNT_DELEGATES == 1
        m_deletor = nullptr;
    }

    // Assignment operators.
    /*!
     * WARNING: The object pointer is directly copied, rather than a deep copy of the object itself. Neuter this new instance or active
     * ref counting if you don't want it prematurely deleting the object.
     */
    Delegate& operator=(const Delegate& delegate) {
        m_object   = delegate.m_object;
        m_executor = delegate.m_executor;
        m_deletor  = delegate.m_deletor;
        if (m_object) {
            m_memberFunction = delegate.m_memberFunction;
#if REFCOUNT_DELEGATES == 1
            if (m_deletor) {
                ++m_deletor->refCount;
#   if PRINT_REFCOUNT_DELEGATES == 1
                // TODO(Matthew): Member function pointers can't be accurately printed through a cast to void*.
                printf("There are now %hu delegates with object: %p, and function pointer: %p.\n", m_deletor->refCount, (void*)m_object, (void*)m_function);
#   endif // PRINT_REFCOUNT_DELEGATES == 1
            }
#endif // REFCOUNT_DELEGATES == 1
        } else {
            m_function = delegate.m_function;
        }
        return *this;
    }
    Delegate& operator=(Delegate&& delegate) {
        m_object   = delegate.m_object;
        if (m_object) {
            m_memberFunction = delegate.m_memberFunction;
        } else {
            m_function = delegate.m_function;
        }
        m_executor = delegate.m_executor;
        m_deletor  = delegate.m_deletor;
        delegate.m_deletor  = nullptr;
        delegate.m_executor = nullptr;
        return *this;
    }

    // Trigger the delegated function with the provided arguments.
    ReturnType invoke(Parameters... parameters) const {
        return m_executor(m_object, m_memberFunction, parameters...);
    }
    // operator() is an alias of invoke.
    ReturnType operator()(Parameters... parameters) const {
        return m_executor(m_object, m_memberFunction, parameters...);
    }

    // Equality and not-equality operators to compare delegates.
    bool operator==(const Delegate& other) const {
        return (m_object   == other.m_object)
            && (m_function == other.m_function);
    }
    bool operator!=(const Delegate& delegate) const {
        return !(*this == delegate);
    }

    /****************************************************************************\
     *                     Creation Functions for Delegates                     *
     *                     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                     *
     * There are far more creation functions than might seem sensible, however  *
     * this is with reason. There are two default kinds:                        *
     *                                                                          *
     *     create:                                                              *
     *         - Creates a delegate without copying any object passed in (if    *
     *           any is passed in). Member functions are called on the object   *
     *           originally passed in, so ensure it is still existent on calls. *
     *     createCopy:                                                          *
     *         - Creates a delegate with a copy of the object passed in. The    *
     *           memory of this object is managed by this class. As such, no    *
     *           concern is needed regarding the lifetime of the original       *
     *           object passed in.                                              *
     *                                                                          *
     * In both cases, if only an object is supplied, we attempt to build a      *
     * delegate using the () operator as the callable. This leads to a          *
     * complexity in certain cases we will explain later.                       *
     *                                                                          *
     * To fix an issue underlying this complexity - the issue of choosing one   *
     * of two functions with the same signature except for one having the const *
     * qualifier - we provide the following additional functions:               *
     *                                                                          *
     *     create{Const|NonConst}:                                              *
     *         - Same as create except in the case of Const we choose the const *
     *           qualified overload, and if NonConst we choose the non-const    *
     *           overload.                                                      *
     *     create{Const|NonConst}Copy:                                          *
     *         - Same as createCopy except in the case of Const we choose the   *
     *           const qualified overload, and if NonConst we choose the        *
     *           non-const overload.                                            *
     *                                                                          *
     * For each and every one of these functions that takes an object as a      *
     * parameter, we provide both a version taking the object by non-const      *
     * pointer, and a version taking the object by const pointer.               *
    \****************************************************************************/

    /**************************\
     *    Static Functions    *
    \**************************/

    static Delegate create(StaticFunction function) {
        return Delegate(*(GenericFunction*)&function, &execute);
    }

    /*********************************************\
     *    Member Functions & Functor Classes     *
     *           (Object Reference)              *
    \*********************************************/

        /****
          Non-const Function
                         ****/

    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate create(SpecificClass* object, MemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>);
    }
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createNonConst(SpecificClass* object, MemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>);
    }

        /****
          Const Function
                     ****/

    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate create(SpecificClass* object, ConstMemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>);
    }
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createConst(SpecificClass* object, ConstMemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>);
    }

    /*******************************************\
     *    Member Functions, Functor Classes    *
     *               & Lambdas                 *
     *            (Object Owned)               *
    \*******************************************/

        /****
          Non-const Function
                         ****/

#if REFCOUNT_DELEGATES == 1
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createOwn(SpecificClass* object, MemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, new DeletorStruct( { &destroy<DecayClass>, 0 } ));
    }
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createNonConstOwn(SpecificClass* object, MemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, new DeletorStruct( { &destroy<DecayClass>, 0 } ));
    }
#else
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createOwn(SpecificClass* object, MemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, &destroy<DecayClass>);
    }
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createNonConstOwn(SpecificClass* object, MemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, &destroy<DecayClass>);
    }
#endif // REFCOUNT_DELEGATES == 1

        /****
          Const Function
                     ****/


#if REFCOUNT_DELEGATES == 1
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createOwn(SpecificClass* object, ConstMemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, new DeletorStruct( { &destroy<DecayClass>, 0 } ));
    }
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createNonConstOwn(SpecificClass* object, ConstMemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, new DeletorStruct( { &destroy<DecayClass>, 0 } ));
    }
#else
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createOwn(SpecificClass* object, ConstMemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, &destroy<DecayClass>);
    }
    template<typename SpecificClass, typename DecayClass = typename std::decay<SpecificClass>::type,
                typename = typename std::enable_if<std::is_class<DecayClass>::value>::type>
    static Delegate createNonConstOwn(SpecificClass* object, ConstMemberFunction<DecayClass> function) {
        return Delegate(static_cast<DecayClass*>(object), *(GenericMemberFunction*)&function, &executeWithObject<DecayClass>, &destroy<DecayClass>);
    }
#endif // REFCOUNT_DELEGATES == 1
protected:
    // Execution routine for static functions.
    static ReturnType execute(GenericObject, GenericMemberFunction function, Parameters... parameters) {
        // Use type punning to get function pointer kind desired.
        // Warning: Technically this can fail dependent on compiler, compilation options etc., but on all tested compilers this works as expected.
        //          We are initially storing the static function pointer that we are casting out of the provided member function pointer in the static function
        //          pointer union field, so it is very likely this will work for most, if not all, scenarios.
        StaticFunction operation = *(StaticFunction*)&function;

        return operation(parameters...);
    }
    // Execution routine for member functions (const or not).
    template<typename SpecificClass, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
    static ReturnType executeWithObject(GenericObject object, GenericMemberFunction function, Parameters... parameters) {
        // Cast generic object and member function pointers to specific type pointers then invoke the function.
        MemberFunction<SpecificClass> operation = (MemberFunction<SpecificClass>)function;
        SpecificClass*                   target = (SpecificClass*)object;

        return (target->*operation)(parameters...);
    }
    // Deletion routine for functors.
    template<typename SpecificClass>
    static void destroy(GenericObject object) {
        delete (SpecificClass*)object;
    }

    GenericObject m_object;
    union {
        GenericFunction       m_function;
        GenericMemberFunction m_memberFunction;
    };
    Executor m_executor;
    Deletor  m_deletor;
};
template<typename ReturnType, typename ...Parameters>
const Delegate<ReturnType, Parameters...> NilDelegate = Delegate<ReturnType, Parameters...>();

/****************************************************************************\
 *                 Helper for Template Parameter Inference                  *
\****************************************************************************/

// This is necessary for inferring the signature of the () operator for
// functor objects such as lambdas and callable classes in general.
template<typename SpecificClass>
struct _DelegateType : _DelegateType<decltype(&SpecificClass::operator())> {};

// We provide two templated definitions, one for const operator() and one for non-const.
template<typename SpecificClass, typename ReturnType, typename ...Parameters>
struct _DelegateType<ReturnType(SpecificClass::*)(Parameters...) const> {
public:
    using type = Delegate<ReturnType, Parameters...>;    
};
template<typename SpecificClass, typename ReturnType, typename ...Parameters>
struct _DelegateType<ReturnType(SpecificClass::*)(Parameters...)> {
public:
    using type = Delegate<ReturnType, Parameters...>;    
};

template<typename SpecificClass, typename Check = void>
struct DelegateType {};

template<typename SpecificClass>
struct DelegateType<SpecificClass, typename std::enable_if<std::is_class<SpecificClass>::value>::type> : public _DelegateType<SpecificClass> {};


/****************************************************************************\
 *                     Creation Routines for Delegates                      *
 *                     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                      *
 * These following routines allow for creating both ordinary delegates and  *
 * functor delegates. The benefit of these methods is they allow us to take *
 * advantage of template parameter inference, with significantly reduced    *
 * boilerplate in the rest of our code.                                     *
 *                                                                          *
 * We provide two kinds of routine:                                         *
 *                                                                          *
 *     makeDelegate:                                                        *
 *         - Creates a delegate without copying the provided object (if an  *
 *           object is provided). In the case of member functions, the      *
 *           provided object must still exist when the delegate is called.  *
 *     makeFunctor:                                                         *
 *         - Creates a delegate, copying the provided object. We call       *
 *           the member function provided through the copied object, which  *
 *           is itself managed by the Delegate class. The provided  object  *
 *           thus need not exist at the calltime of the delegate.           *
 *                                                                          *
 * There are, however, some complexities with these functions.              *
 *                                                                          *
 * Firstly, if we are passed a function handle that has overloads with      *
 * different signatures, we cannot choose for you which to select and, due  *
 * to the limitations of template parameter inference, you can't tell us by *
 * providing a well-defined delegate type to return into.                   *
 *                                                                          *
 * Instead, you must specify explicitly with the functions, for example:    *
 *                                                                          *
 *   Delegate<int, int> myDelegate                                          *
 *                         = makeDelegate<int, int>(myObj, &MyObj::func);   *
 *   or, equivalently:                                                      *
 *      auto myDelegate = makeDelegate<int, int>(myObj, &MyObj::func);      *
 *                                                                          *
 * In addition, if we are passed a function handle that has overloads with  *
 * the same signature but for a const qualifier, we cannot differentiate    *
 * these functions and so cannot choose on your behalf. We provide four     *
 * more functions to solve this:                                            *
 *                                                                          *
 *     make{Const|NonConst}Delegate:                                        *
 *         - Same as makeDelegate except if Const we will choose the const  *
 *           qualified overload, if NonConst we will choose the non-const   *
 *           overload.                                                      *
 *     make{Const|NonConst}Functor:                                         *
 *         - Same as makeFunctor except if Const we will choose the const   *
 *           qualified overload, if NonConst we will choose the non-const   *
 *           overload.                                                      *
 *                                                                          *
 * In the specific case of providing just an object, where we default to    *
 * choosing the () operator, both the above problems play a role. If there  *
 * are overloads of the () operator in the chosen signature but one with a  *
 * const qualifier and one without, then it is necessary to also use the    *
 * make{Const|NonConst}{Delegate|Functor} functions.                        *
 *                                                                          *
 * Further, as the () operator is FUNKY, and template inference completely  *
 * collapses when using it, you must - no matter whether or not there are   *
 * any overloads of the signature - provide the return type and parameter   *
 * list.                                                                    *
 *                                                                          *
 * As an example, for a case where we have a const and non-const version    *
 * of an operator with signature int(MyClass::*)(int, int), we would use:   *
 *                                                                          *
 *       auto myDelegate = makeConstFunctor<int, int, int>(myObj);          *
 *                                                                          *
 * where we have chosen to make a delegate of the const qualified version   *
 * of the () operator.                                                      *
 *                                                                          *
 * In general, the most apt uses of the delegate system need not use these  *
 * verbose solutions. However, in the eventualities where they are needed,  *
 * they are provided.                                                       *
\****************************************************************************/


/**************************\
 *    Static Functions    *
\**************************/

template<typename ReturnType, typename ...Parameters>
Delegate<ReturnType, Parameters...> makeDelegate(ReturnType(*function)(Parameters...)) {
    return Delegate<ReturnType, Parameters...>::create(function);
}

/*********************************************\
 *    Member Functions (Object Reference)    *
\*********************************************/

    /****
      Non-const Function
                     ****/

template<typename SpecificClass, typename ReturnType, typename ...Parameters,
            typename = typename std::enable_if<std::is_class<typename std::decay<SpecificClass>::type>::value>::type>
Delegate<ReturnType, Parameters...> makeDelegate(SpecificClass* object, ReturnType(SpecificClass::*function)(Parameters...)) {
    return Delegate<ReturnType, Parameters...>::template create<SpecificClass>(object, function);
}
template<typename SpecificClass, typename ReturnType, typename ...Parameters,
            typename = typename std::enable_if<std::is_class<typename std::decay<SpecificClass>::type>::value>::type>
Delegate<ReturnType, Parameters...> makeNonConstDelegate(SpecificClass* object, ReturnType(SpecificClass::*function)(Parameters...)) {
    return Delegate<ReturnType, Parameters...>::template createNonConst<SpecificClass>(object, function);
}

    /****
      Const Function
                 ****/

template<typename SpecificClass, typename ReturnType, typename ...Parameters,
            typename = typename std::enable_if<std::is_class<typename std::decay<SpecificClass>::type>::value>::type>
Delegate<ReturnType, Parameters...> makeDelegate(SpecificClass* object, ReturnType(SpecificClass::*function)(Parameters...) const) {
    return Delegate<ReturnType, Parameters...>::template create<SpecificClass>(object, function);
}
template<typename SpecificClass, typename ReturnType, typename ...Parameters,
            typename = typename std::enable_if<std::is_class<typename std::decay<SpecificClass>::type>::value>::type>
Delegate<ReturnType, Parameters...> makeConstDelegate(SpecificClass* object, ReturnType(SpecificClass::*function)(Parameters...) const) {
    return Delegate<ReturnType, Parameters...>::template createConst<SpecificClass>(object, function);
}

/********************************************\
 *    Functor Classes (Object Reference)    *
\********************************************/

// Const or non-const operator().
template<typename SpecificClass, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
typename DelegateType<DecayedClass>::type makeDelegate(SpecificClass* object) {
    return DelegateType<DecayedClass>::type::template create<DecayedClass>(object, &DecayedClass::operator());
}
// Const operator().
template<typename ReturnType, typename ...Parameters, typename SpecificClass, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeConstDelegate(SpecificClass* object) {
    using Func = ReturnType(DecayedClass::*)(Parameters...) const;

    Func function = &DecayedClass::operator();

    return Delegate<ReturnType, Parameters...>::template create<DecayedClass>(object, function);
}
// Non-const operator().
template<typename ReturnType, typename ...Parameters, typename SpecificClass, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeNonConstDelegate(SpecificClass* object) {
    using Func = ReturnType(DecayedClass::*)(Parameters...);

    Func function = &DecayedClass::operator();

    return Delegate<ReturnType, Parameters...>::template create<DecayedClass>(object, function);
}

/***************************************\
 *   Member Functions (Object Owned)   *
\***************************************/

    /****
      Non-const Function
                     ****/

template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeFunctor(SpecificClass&& object, ReturnType(SpecificClass::*function)(Parameters...)) {
    return Delegate<ReturnType, Parameters...>::template createOwn<DecayedClass>(new DecayedClass(std::forward<SpecificClass>(object)), function);
}
template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeNonConstFunctor(SpecificClass&& object, ReturnType(SpecificClass::*function)(Parameters...)) {
    return Delegate<ReturnType, Parameters...>::template createNonConstOwn<DecayedClass>(new DecayedClass(std::forward<SpecificClass>(object)), function);
}

    /****
      Const Function
                 ****/

template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeFunctor(SpecificClass&& object, ReturnType(SpecificClass::*function)(Parameters...) const) {
    return Delegate<ReturnType, Parameters...>::template createOwn<DecayedClass>(new DecayedClass(std::forward<SpecificClass>(object)), function);
}
template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeConstFunctor(SpecificClass&& object, ReturnType(SpecificClass::*function)(Parameters...) const) {
    return Delegate<ReturnType, Parameters...>::template createConstOwn<DecayedClass>(new DecayedClass(std::forward<SpecificClass>(object)), function);
}

/**************************************\
 *   Functor Classes                  *
 *      & Lambdas     (Object Owned)  *
\**************************************/

// Const or non-const operator().
template<typename SpecificClass, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
typename DelegateType<DecayedClass>::type makeFunctor(SpecificClass&& object) {
    return DelegateType<DecayedClass>::type::template createOwn<DecayedClass>(new DecayedClass(std::forward<SpecificClass>(object)), &DecayedClass::operator());
}
// Const operator().
template<typename ReturnType, typename ...Parameters, typename SpecificClass, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeConstFunctor(SpecificClass&& object) {
    using Func = ReturnType(DecayedClass::*)(Parameters...) const;

    Func function = &DecayedClass::operator();

    return Delegate<ReturnType, Parameters...>::template createOwn<DecayedClass>(new DecayedClass(std::forward<SpecificClass>(object)), function);
}
// Non-const operator().
template<typename ReturnType, typename ...Parameters, typename SpecificClass, typename DecayedClass = typename std::decay<SpecificClass>::type,
            typename = typename std::enable_if<std::is_class<DecayedClass>::value>::type>
Delegate<ReturnType, Parameters...> makeNonConstFunctor(SpecificClass&& object) {
    using Func = ReturnType(DecayedClass::*)(Parameters...);

    Func function = &DecayedClass::operator();

    return Delegate<ReturnType, Parameters...>::template createOwn<DecayedClass>(new DecayedClass(std::forward<SpecificClass>(object)), function);
}

#endif // !Vorb_Delegate_h__
