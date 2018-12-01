#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Script_

#include <include/script/lua/Environment.h>
#include <include/script/ConsoleBackend.hpp>

TEST(CreateEnvironment) {
    vscript::lua::Environment env;
    if (!env.getHandle()) return false;
    printf("Handle: %lx\n", (ptrdiff_t)env.getHandle());
    return true;
}

void sum(void* ret, int a, int b) {
    puts("What?");
    int* r = (int*)ret;
    *r = a + b + b;
}

i32 stuff(i32 a, i32 b, i32 c) {
    return a + b * c;
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
    vscript::lua::Environment env;

    TestObject to;
    env.addCDelegate("TestObject_increment", makeDelegate(to, &TestObject::increment));

    env.addCDelegate("getUnitX", makeDelegate(getUnitX));
    env.addCDelegate("stuffs", makeDelegate(stuff));
    env.addCDelegate("printVec", makeDelegate(printVec));
    env.setNamespaces("Test", "Namespace");
    env.addCDelegate("valEnum", makeDelegate(valEnum));
    env.addCDelegate("valPtr", makeDelegate(valPtr));
    env.addCDelegate("valRef", makeDelegate(valRef));

    if (!env.run("data/add.lua")) return false;

    i32 value;
    auto f = env.template getScriptDelegate<i32, i32, i32>("add");
    value = f(1, 5);
    value = f(1, 6);
    value = f(1, 7);
    value = f(1, 8);
    auto f2 = env.template getScriptDelegate<i32, i32, i32>("add2");
    return value == 13 && to.x == 4 && f2(5, 7) == 12;
}

ui32 consoleColor = 0;
#ifdef VORB_OS_WINDOWS
HANDLE hndConsole = 0;
#endif
void writeError(const cString msg) {
    fputs(msg, stderr);
}
void luaSleep(ui32 ms) {
    Sleep(ms);
}
void setColor(ui32 color) {
    if (consoleColor != color) {
        consoleColor = color;
        fflush(stdout);
        fflush(stderr);
#ifdef VORB_OS_WINDOWS
        SetConsoleTextAttribute(hndConsole, color);
#endif
    }
}
void onREPLOut(Sender, const cString msg) {
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    fputs(msg, stdout);
}
void onREPLErr(Sender, const cString msg) {
    setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    fputs(msg, stderr);
}

TEST(REPL) {
#ifdef VORB_OS_WINDOWS
    hndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
    
    // Create REPL
    vscript::lua::Environment env = {};
    env.addCDelegate("printErr", makeDelegate(writeError));
    env.addCDelegate("sleep",    makeDelegate(luaSleep));
    vscript::ConsoleBackend<vscript::lua::Environment> repl();
    repl.init(&env);
    repl.onConsoleOutput.out += makeDelegate(onREPLOut);
    repl.onConsoleOutput.err += makeDelegate(onREPLErr);

    char buf[1024];
    while (true) {
        // Show input prompt
        setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        fputs(">>> ", stdout);
        fflush(stdout);

        // Get input
        setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        std::cin.getline(buf, 1024);

        // Execute a command
        if (strcmp(buf, "q") == 0) break;
        repl.invokeCommand(buf);
        onREPLOut(nullptr, "\n");
    }

    return true;
}
