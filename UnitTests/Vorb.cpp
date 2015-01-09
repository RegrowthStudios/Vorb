#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_

#include <include/Vorb.h>

TEST(InitDispose) {
    if (vorb::init(vorb::InitParam::ALL) != vorb::InitParam::ALL) return false;
    return vorb::dispose(vorb::InitParam::ALL) == vorb::InitParam::ALL;
}