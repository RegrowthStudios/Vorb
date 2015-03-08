#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Script_

#include <include/script/Environment.h>
#include <include/script/Script.h>

TEST(CreateEnvironment) {
    vscript::Environment env;
    if (!env.getHandle()) return false;
    printf("Handle: %lx\n", (ptrdiff_t)env.getHandle());
    return true;
}

TEST(LoadScript) {
    vscript::Environment env;
    if (!env.load("data/add.lua")) return false;

    i32 value;
    vscript::callReturn(env, "add", &value, 1, 1);
    return value == 2;
}
