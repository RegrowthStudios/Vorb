#include <Vorb/PtrRecycler.hpp>

class CXTOR {
public:
    CXTOR() {
        count++;
    }
    ~CXTOR() {
        count--;
    }

    static i32 count;
};
i32 CXTOR::count = 0;

int main(int argc, cString* argv) {
    
    bool b = true;

    { // Test how pointers work
        PtrRecycler<i32> pr;

        // Make and recycle a value
        i32* val = pr.create(10);
        b &= (*val == 10);
        pr.recycle(val);

        // Make sure we pop off of the recycled list
        b &= (pr.create(100) == val);
        b &= (*val == 100);
        pr.recycle(val);
    }

    { // Test number of times construct and destructor are called
        PtrRecycler<CXTOR> pr;

        // Assure constructors are called
        b &= (CXTOR::count == 0);
        pr.create();
        pr.create();
        pr.create();
        b &= (CXTOR::count == 3);

        // Random allocations
        pr.recycle(pr.create());
        pr.create();
        pr.create();
        pr.recycle(pr.create());
        pr.recycle(pr.create());
        pr.create();
        pr.create();
        pr.recycle(pr.create());
        pr.create();
        pr.create();
        pr.recycle(pr.create());

        // Everything automatically freed at end
    }

    // In the end, all objects created and destroyed
    b &= (CXTOR::count == 0);

    return 0;
}

