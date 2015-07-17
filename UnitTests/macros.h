#pragma once

#ifndef macros_h__
#define macros_h__

#include "Tests.h"

#define XSTR(s) STR(s)
#define STR(s) #s

#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

#define IMPL_TEST_ABS_HIDDEN(B, N) \
namespace UnitTestsFuncs { \
    bool UT_##B##N(); \
    bool UT_##B##N##_init = UnitTests::Adder::TestsAdder::addTest((XSTR(PPCAT(B, N))), UT_##B##N); \
} \
bool UnitTestsFuncs::UT_##B##N()

#define TEST(NAME) IMPL_TEST_ABS_HIDDEN(UNIT_TEST_BATCH, NAME)

#define UNIT_TEST_BATCH General_

#define DO_2X(EX) EX; EX
#define DO_4X(EX) DO_2X(EX); DO_2X(EX)
#define DO_8X(EX) DO_4X(EX); DO_4X(EX)
#define DO_16X(EX) DO_8X(EX); DO_8X(EX)
#define DO_32X(EX) DO_16X(EX); DO_16X(EX)
#define DO_64X(EX) DO_32X(EX); DO_32X(EX)
#define DO_128X(EX) DO_64X(EX); DO_64X(EX)
#define DO_256X(EX) DO_128X(EX); DO_128X(EX)

#endif // macros_h__
