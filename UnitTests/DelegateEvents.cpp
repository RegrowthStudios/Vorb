#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH DE_

#include "include/Event.hpp"

#include "TestClasses.h"

TEST(TypelessMemberFunctionAccess) {
    TestClassBase valueA = {};
    TestClassDerived valueB = {};

    TypelessMember* typelessA = typeless(&valueA);
    TypelessMember* typelessB = typeless(&valueB);

    auto funcVirtualA = TypelessMember::func(&TestClassBase::virtualFunc);
    auto funcNonVirtualBaseA = TypelessMember::func(&TestClassBase::nonVirtualBaseFunc);
    auto funcOverridenA = TypelessMember::func(&TestClassBase::overridenFunc);
    auto funcVirtualB = TypelessMember::func(&TestClassDerived::virtualFunc);
    auto funcNonVirtualBaseB = TypelessMember::func(&TestClassDerived::nonVirtualBaseFunc);
    auto funcOverridenB = TypelessMember::func(&TestClassDerived::overridenFunc);

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
    TestClassDerived value = {};
    value.x = 10;

    // Grab typeless pointers
    i32 TypelessMember::* ptr = TypelessMember::value(&TestClassDerived::x);
    i32 TypelessMember::* ptrOffset = TypelessMember::value<i32>(offsetof(TestClassDerived, x));
    i32 TypelessMember::* ptrBad = TypelessMember::value(&TestClassBase::x);
    i32 TypelessMember::* ptrBadOffset = TypelessMember::value<i32>(offsetof(TestClassBase, x));
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
    TestClassBase valueA = {};
    TestClassDerived valueB = {};

#define TEST_VALUE(OBJ, CLASS, FUNC, VALUE) \
    { \
        auto d = makeRDelegate(OBJ, &CLASS::FUNC); \
        if (d() != VALUE) { \
            printf("Delegate ("#OBJ","#CLASS"::"#FUNC") != "#VALUE"\n"); \
            return false; \
        } \
    }
    TEST_VALUE(valueA, TestClassBase, virtualFunc, 1);
    TEST_VALUE(valueA, TestClassBase, nonVirtualBaseFunc, 2);
    TEST_VALUE(valueA, TestClassBase, overridenFunc, 3);
    TEST_VALUE(valueB, TestClassDerived, virtualFunc, 4);
    TEST_VALUE(valueB, TestClassDerived, nonVirtualBaseFunc, 5);
    TEST_VALUE(valueB, TestClassDerived, overridenFunc, 6);
    TEST_VALUE(*((TestClassBase*)&valueB), TestClassBase, virtualFunc, 4);
    TEST_VALUE(*((TestClassBase*)&valueB), TestClassBase, nonVirtualBaseFunc, 2);
    TEST_VALUE(*((TestClassBase*)&valueB), TestClassBase, overridenFunc, 3);
#undef TEST_VALUE
    
    return true;
}

TEST(Convention1) {
    RDelegate<bool, bool> v = makeRDelegate(getOppositeValue);
    return v(false);
}

TEST(Convention2) {
    TestClassBase obj;
    RDelegate<bool, bool> v = makeRDelegate(obj, &TestClassBase::getOppositeValue);
    return v(false);
}

TEST(Convention3) {
    TestClassBase obj;
    RDelegate<bool, bool>* v = makeRFunctor(obj, &TestClassBase::getOppositeValue);
    bool a = v->invoke(false);
    delete v;
    return a;
}

TEST(OverridenFuncs) {
    TestClassBase objBase;
    TestClassDerived objDerived;

    auto v1 = makeRDelegate(objBase, &TestClassBase::overridenFunc);
    auto v2 = makeRDelegate(objDerived, &TestClassDerived::overridenFunc);
    auto v3 = makeRDelegate((TestClassBase&)objDerived, &TestClassBase::overridenFunc);

    return v1() == 3 && v2() == 6 && v3() == 3;
}

TEST(VirtualFuncs) {
    TestClassBase objBase;
    TestClassDerived objDerived;

    auto v1 = makeRDelegate(objBase, &TestClassBase::virtualFunc);
    auto v2 = makeRDelegate(objDerived, &TestClassDerived::virtualFunc);
    auto v3 = makeRDelegate((TestClassBase&)objDerived, &TestClassBase::virtualFunc);

    return v1() == 1 && v2() == 4 && v3() == 4;
}

TEST(NonVirtualBaseFuncs) {
    TestClassBase objBase;
    TestClassDerived objDerived;
    TestClassSuperDerived objSuperDerived;

    auto v1 = makeRDelegate(objBase, &TestClassBase::nonVirtualBaseFunc);
    auto v2 = makeRDelegate(objDerived, &TestClassDerived::nonVirtualBaseFunc);
    auto v3 = makeRDelegate((TestClassBase&)objDerived, &TestClassBase::nonVirtualBaseFunc);
    auto v4 = makeRDelegate(objSuperDerived, &TestClassSuperDerived::nonVirtualBaseFunc);
    auto v5 = makeRDelegate((TestClassDerived&)objSuperDerived, &TestClassDerived::nonVirtualBaseFunc);
    auto v6 = makeRDelegate((TestClassBase&)objSuperDerived, &TestClassBase::nonVirtualBaseFunc);

    return v1() == 2 && v2() == 5 && v3() == 2 && v4() == 7 && v5() == 7 && v6() == 2;
}
