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

enum class MyEnum {
    RED,
    DOG
};
void valEnum(MyEnum e) {
    puts("Called enum func");
}
void valPtr(int* i) {
    puts("Called pointer func");
}
void valRef(int& i) {
    puts("Called ref func");
}

class TestObject {
public:
    int increment() {
        return ++x;
    }

    int x = 0;
};

TEST(LoadScript) {
    vscript::Environment env;
    auto c = vscript::fromFunction<sum>(sum);

    TestObject to;
    auto df = makeRDelegate(to, &TestObject::increment);
    env.addCRDelegate("TestObject_increment", &df);


    env.addCFunction("valEnum", vscript::fromFunction<valEnum>(valEnum));
    env.addCFunction("valPtr", vscript::fromFunction<valPtr>(valPtr));
    env.addCFunction("valRef", vscript::fromFunction<valRef>(valRef));

    if (!env.load("data/add.lua")) return false;

    i32 value;
    vscript::callReturn(env, "add", &value, 1, 5);
    vscript::callReturn(env, "add", &value, 1, 6);
    vscript::callReturn(env, "add", &value, 1, 7);
    vscript::callReturn(env, "add", &value, 1, 8);
    return value == 13 && to.x == 4;
}
