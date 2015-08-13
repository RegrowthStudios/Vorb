#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH DE_

#include "include/Events.hpp"

class A {
public:
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
