#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH QD_

#include <include/Event.hpp>

class DEED {
public:
    void incr() {
        x++;
    }

    int x = 1;
};

void adder(int& r, int a, int b) {
    r = a + b;
}


Delegate<void, int> sumthing() {
    return makeDelegate([] (int n) {
        printf("%d\n", n);
    });
}
int* shite() {
    int v = 5;
    return &v;
}

int f(int& v) {
    return 0;
}

TEST(1) {
    DEED deed;
    int r = 10;

    {
        auto rd = makeRDelegate(shite);
        int* v = rd();
        *v = 5;
    }

    {
        auto rd = makeDelegate(&deed, &DEED::incr);
        rd();
        rd();
    }

    {
        Delegate<void>* ftor;
        {
            auto fr = [&] () {
                r += 10;
            };
            ftor = makeFunctor([fr] () {
                fr();
            });
        }
        ftor->invoke();
        delete ftor;
    }
    return true;
}