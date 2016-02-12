#pragma once

#ifndef TestClasses_h__
#define TestClasses_h__

bool performComputation(i32 v) {
    static i32 shifty = ~0;

    i32 x = shifty;
    v &= 31;
    x = (x >> v) | x;

    // Hopefully it takes a while for compilers to figure out that this will always return true
    return x != 0;
}
bool getOppositeValue(bool v) {
    return !v;
}

class TestClassBase {
public:
    TestClassBase() {
        printf("%08llx --- CTOR\n", this);
        fflush(stdout);
    }
    TestClassBase(const TestClassBase&) {
        printf("%08llx --- COPY\n", this);
        fflush(stdout);
    }
    TestClassBase(TestClassBase&&) {
        printf("%08llx --- MOVE\n", this);
        fflush(stdout);
    }
    TestClassBase& operator=(const TestClassBase&) {
        printf("%08llx --- COPY ASSIGN\n", this);
        fflush(stdout);
        return *this;
    }
    TestClassBase& operator=(TestClassBase&&) {
        printf("%08llx --- MOVE ASSIGN\n" , this);
        fflush(stdout);
        return *this;
    }
    virtual ~TestClassBase() {
        printf("%08llx --- DTOR\n", this);
        fflush(stdout);
    }

    bool getOppositeValue(bool v) {
        return v ? !performComputation(1) : performComputation(0);
    }

    virtual i32 virtualFunc() {
        return performComputation(1) ? 1 : 0;
    }
    i32 nonVirtualBaseFunc() {
        return performComputation(2) ? 2 : 0;
    }
    i32 overridenFunc() {
        return performComputation(3) ? 3 : 0;
    }

    i32 x;
};

class TestClassDerived : public TestClassBase {
public:
    virtual i32 virtualFunc() {
        return performComputation(4) ? 4 : 0;
    }
    virtual i32 nonVirtualBaseFunc() {
        return performComputation(5) ? 5 : 0;
    }
    i32 overridenFunc() {
        return performComputation(6) ? 6 : 0;
    }

    i32 notX;
    i32 x;
};

class TestClassSuperDerived : public TestClassDerived {
public:
    virtual i32 nonVirtualBaseFunc() {
        return performComputation(5) ? 7 : 0;
    }

    i32 notX;
    i32 x;
};

#endif // !TestClasses_h__
