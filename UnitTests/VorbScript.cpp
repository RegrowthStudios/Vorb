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

void sum(void* ret, int a, int b) {
    puts("What?");
    int* r = (int*)ret;
    *r = a + b + b;
}

class TestObject {
public:
    void increment() {
        x++;
    }

    int x = 0;
};

TEST(LoadScript) {
    vscript::Environment env;
    auto c = vscript::fromFunction<void(*)(void*, int, int), sum, void*, int, int>();

    TestObject to;
    auto df = makeDelegate(to, &TestObject::increment);
    env.addCDelegate("TestObject_increment", &df);

    i32 ov = 0;
    env.addValue("myData", &ov);

    if (!env.load("data/add.lua")) return false;

    i32 value;
    vscript::callReturn(env, "add", &value, 1, 5);
    vscript::callReturn(env, "add", &value, 1, 6);
    vscript::callReturn(env, "add", &value, 1, 7);
    vscript::callReturn(env, "add", &value, 1, 8);
    return value == 9 && to.x == 4;
}
