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

f32v3 getUnitX() {
    return f32v3(1, 0, 0);
}
void printVec(f32v3 v) {
    printf("%f,%f,%f\n", v.x, v.y, v.z);
}

class TestObject {
public:
    int increment() {
        return ++x;
    }

    int x = 0;
};

class TC {
public:
    TC() {
        x = 0;
        puts("TC CTOR");
    }
    ~TC() {
        printf("TC DTOR %d\n", x);
    }

    TC(const TC& o) {
        x = o.x;
        puts("TC COPY");
    }
    TC& operator=(const TC& o) {
        x = o.x;
        puts("TC ATOR");
        return *this;
    }
    TC(TC&& o) {
        x = o.x;
        o.x = 666;
        puts("TC RCOPY");
    }

    i32 x;
};

TC getTC() {
    TC v {};
    v.x = 10;
    return std::move(v);
}

TEST(MoveValue) {
    TC v = getTC();
    return true;
}

TEST(LoadScript) {
    vscript::Environment env;
    auto c = vscript::fromFunction<sum>(sum);

    TestObject to;
    auto df = makeRDelegate(to, &TestObject::increment);
    env.addCRDelegate("TestObject_increment", &df);

    auto gux = makeRDelegate(getUnitX);
    env.addCRDelegate("getUnitX", &gux);
    env.addCFunction("printVec", vscript::fromFunction<printVec>(printVec));
    env.addCFunction("valEnum", vscript::fromFunction<valEnum>(valEnum));
    env.addCFunction("valPtr", vscript::fromFunction<valPtr>(valPtr));
    env.addCFunction("valRef", vscript::fromFunction<valRef>(valRef));

    if (!env.load("data/add.lua")) return false;

    i32 value;
    auto f = env["add"].as<i32>();
    value = f(1, 5);
    value = f(1, 6);
    value = f(1, 7);
    value = f(1, 8);
    return value == 13 && to.x == 4;
}
