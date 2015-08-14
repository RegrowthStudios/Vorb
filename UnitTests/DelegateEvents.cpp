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
};


TEST(BlankFunctionAccess) {
    A value = {};

    void* obj = &value;
    PointerCast<bool (A::*)(bool)> fObj = { &A::getOppositeValue };

    BlankTemplate* blankedObject = (BlankTemplate*)obj;
    PointerCast<bool (BlankTemplate::*)(bool)> blankedFunction = { fObj.ptr };
    
    return (blankedObject->*blankedFunction.value)(false);
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
