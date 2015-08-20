#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH DE_

#include "include/Events.hpp"

bool getOppositeValue(bool v) {
    return !v;
}

class A {
public:
    A() {
        puts("CTOR");
        fflush(stdout);
    }
    A(const A&) {
        puts("Copy CTOR");
        fflush(stdout);
    }
    A(A&&) {
        puts("Move CTOR");
        fflush(stdout);
    }
    A& operator=(const A&) {
        puts("Copy ASGN");
        fflush(stdout);
        return *this;
    }
    A& operator=(A&&) {
        puts("Move ASGN");
        fflush(stdout);
        return *this;
    }
    virtual ~A() {
        puts("I've been destroyed");
        fflush(stdout);
    }

    bool getOppositeValue(bool v) {
        return !v;
    }

    virtual i32 virtualFunc() {
        return 1;
    }
    i32 nonVirtualBaseFunc() {
        return 2;
    }
    i32 overridenFunc() {
        return 3;
    }

    i32 x;
};

class B : public A {
public:
    virtual i32 virtualFunc() {
        return 4;
    }
    virtual i32 nonVirtualBaseFunc() {
        return 5;
    }
    i32 overridenFunc() {
        return 6;
    }

    i32 notX;
    i32 x;
};

TEST(TypelessMemberFunctionAccess) {
    A valueA = {};
    B valueB = {};

    TypelessMember* typelessA = typeless(&valueA);
    TypelessMember* typelessB = typeless(&valueB);

    auto funcVirtualA = TypelessMember::func(&A::virtualFunc);
    auto funcNonVirtualBaseA = TypelessMember::func(&A::nonVirtualBaseFunc);
    auto funcOverridenA = TypelessMember::func(&A::overridenFunc);
    auto funcVirtualB = TypelessMember::func(&B::virtualFunc);
    auto funcNonVirtualBaseB = TypelessMember::func(&B::nonVirtualBaseFunc);
    auto funcOverridenB = TypelessMember::func(&B::overridenFunc);

#define TEST_VALUE(OBJ, FUNC, VALUE) \
    if ((OBJ->*FUNC)() != VALUE) { \
        printf(#OBJ"->*"#FUNC"() != "#VALUE"\n"); \
        return false; \
            }

    TEST_VALUE(typelessA, funcVirtualA, 1);
    TEST_VALUE(typelessB, funcVirtualB, 4);
    TEST_VALUE(typelessB, funcVirtualA, 4);
    TEST_VALUE(typelessA, funcNonVirtualBaseA, 2);
    TEST_VALUE(typelessB, funcNonVirtualBaseB, 5);
    TEST_VALUE(typelessB, funcNonVirtualBaseA, 2);
    TEST_VALUE(typelessA, funcOverridenA, 3);
    TEST_VALUE(typelessB, funcOverridenB, 6);
    TEST_VALUE(typelessA, funcOverridenB, 6);
    TEST_VALUE(typelessB, funcOverridenA, 3);
#undef TEST_VALUE

    return true;
}

TEST(TypelessMemberValueAccess) {
    // Create a simple object
    B value = {};
    value.x = 10;

    // Grab typeless pointers
    i32 TypelessMember::* ptr = TypelessMember::value(&B::x);
    i32 TypelessMember::* ptrOffset = TypelessMember::value<i32>(offsetof(B, x));
    i32 TypelessMember::* ptrBad = TypelessMember::value(&A::x);
    i32 TypelessMember::* ptrBadOffset = TypelessMember::value<i32>(offsetof(A, x));
    TypelessMember* obj = typeless(&value);
    
    // Test for offset locations
    if (ptr != ptrOffset) {
        printf("Typeless member offset are not equivalent:\n%llx\n%llx\n", ptr, ptrOffset);
        return false;
    }
    if (ptrBad != ptrBadOffset) {
        printf("Typeless member offset are not equivalent:\n%llx\n%llx\n", ptrBad, ptrBadOffset);
        return false;
    }
    if (ptr == ptrBad) {
        printf("Typeless member offset are equivalent:\n%llx\n%llx\n", ptr, ptrBad);
        return false;
    }

    // Perform the member dereference
    int typelessValue = obj->*ptr;

    return typelessValue == 10;
}

TEST(DelegateMemberAccess) {
    A valueA = {};
    B valueB = {};

#define TEST_VALUE(OBJ, CLASS, FUNC, VALUE) \
    { \
        auto d = makeRDelegate(OBJ, &CLASS::FUNC); \
        if (d() != VALUE) { \
            printf("Delegate ("#OBJ","#CLASS"::"#FUNC") != "#VALUE"\n"); \
            return false; \
        } \
    }
    TEST_VALUE(valueA, A, virtualFunc, 1);
    TEST_VALUE(valueA, A, nonVirtualBaseFunc, 2);
    TEST_VALUE(valueA, A, overridenFunc, 3);
    TEST_VALUE(valueB, B, virtualFunc, 4);
    TEST_VALUE(valueB, B, nonVirtualBaseFunc, 5);
    TEST_VALUE(valueB, B, overridenFunc, 6);
    TEST_VALUE(*((A*)&valueB), A, virtualFunc, 4);
    TEST_VALUE(*((A*)&valueB), A, nonVirtualBaseFunc, 2);
    TEST_VALUE(*((A*)&valueB), A, overridenFunc, 3);
#undef TEST_VALUE
    
    return true;
}

TEST(Convention1) {
    RDelegate<bool, bool> v = makeRDelegate(getOppositeValue);
    return v(false);
}

TEST(Convention2) {
    A obj;
    RDelegate<bool, bool> v = makeRDelegate(obj, &A::getOppositeValue);
    return v(false);
}

TEST(Convention3) {
    A obj;
    RDelegate<bool, bool>* v = makeRFunctor(obj, &A::getOppositeValue);
    bool a = v->invoke(false);
    delete v;
    return a;
}
