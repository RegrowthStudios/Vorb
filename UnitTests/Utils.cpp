#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Utils_

#include <random>
#include <thread>

#include <include/Events.hpp>
#include <include/Random.h>
#include <include/RingBuffer.hpp>
#include <include/Timing.h>
#include <include/ScopedTiming.hpp>
#include <include/PtrRecycler.hpp>
#include <include/Endian.h>

TEST(MersenneTwister) {
    f32 v1 = 0.0f, v2 = 0.0f;

    ::Random zr(20);
    std::mt19937 mt(20);


    vorb::AveragedSamplerContext sc1;
    { vorb::ScopedAveragedSampler vorb_scopedtime(sc1);
        for (size_t i = 0; i < 1000000; i++) {
            v1 += mt() / (f32)0xffffffffu;
        }
    }

    vorb::AveragedSamplerContext sc2;
    { vorb::ScopedAveragedSampler vorb_scopedtime(sc2);
    for (size_t i = 0; i < 1000000; i++) {
        v2 += zr.genMT();
    }
    }

    std::cout << sc1.getAccumulatedMicroseconds() << " : " << v1 / 100000.0f << std::endl;
    std::cout << sc2.getAccumulatedMicroseconds() << " : " << v2 / 100000.0f << std::endl;

    return true;
}

class A {
public:
    static vorb::DetailedSamplerContext lifeTimes;
private:
    vorb::ScopedDetailedSampler lifeMeasure { lifeTimes };
};
vorb::DetailedSamplerContext A::lifeTimes;

TEST(LifeTime) {
    A* a = new A;
    Sleep(1000);
    delete a;
    std::cout << "Lifetime: " << A::lifeTimes.getAccumulatedSeconds() << std::endl; // Prints 2
    return true;
}

TEST(MTSampling) {
    vorb::MTDetailedSamplerContext context;

    auto f = [&] () {
        VORB_SAMPLE_SCOPE(context);
        VORB_SAMPLE_SCOPE(context);
        Sleep(1000);
    };

    {
        std::thread t1(f);
        std::thread t2(f);
        std::thread t3(f);
        std::thread t4(f);
        std::thread t5(f);

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
    }

    std::cout << "Accum. Time: " << context.getAccumulatedSeconds() << std::endl;
    std::cout << "Entries:     " << context.getEntryCount() << std::endl;
    std::cout << "Avg. Time:   " << context.getAverageSeconds() << std::endl;
    std::cout << "Min Time:    " << context.getMinElapsedMicroseconds() << std::endl;
    std::cout << "Max Time:    " << context.getMaxElapsedMicroseconds() << std::endl;
    return context.getEntryCount() == 10;
}

TEST(RingBuffer) {
    vorb::ring_buffer<int> q(4);
    if (!q.push(1, 2, 3, 4)) return false;
    if (q.front() != 1) return false;
    q.pop();
    if (q.front() != 2) return false;
    q.pop();
    if (q.front() != 3) return false;
    q.pop();

    if (!q.push(5)) return false;
    q.resize(2);
    if (q.front() != 4) return false;
    q.pop();
    if (q.front() != 5) return false;
    q.pop();

    if (q.size() != 0) return false;

    return true;
}

enum class MyBoolean {
    BFALSE = 0x00,
    BTRUE = 0x01
};
ENUM_CLASS_OPS_INL(MyBoolean, bool)


TEST(EnumOps) {
    MyBoolean b1 = MyBoolean::BFALSE ^ (MyBoolean::BFALSE & MyBoolean::BTRUE);
    if (b1 != MyBoolean::BFALSE) return false;

    b1 += MyBoolean::BTRUE;
    if (b1 != MyBoolean::BTRUE) return false;

    b1 += MyBoolean::BTRUE;
    if (b1 != MyBoolean::BTRUE) return false;

    b1 -= MyBoolean::BFALSE;
    if (b1 != MyBoolean::BTRUE) return false;

    b1 -= MyBoolean::BTRUE;
    return b1 == MyBoolean::BFALSE && (MyBoolean::BTRUE + MyBoolean::BTRUE + MyBoolean::BTRUE == MyBoolean::BTRUE);
}

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

TEST(PtrRecycler) {
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

    return b;
}

class OValue {
public:
    OValue(i32 c) {
        x = c;
    }

    i32 add(i32 a, i32 b) {
        return a + b + x;
    }
    i32 getValue() {
        return x;
    }
private:
    i32 x;
};

TEST(DelegateSpeed) {

    {
        vorb::MTDetailedSamplerContext context;
        OValue ovalue(3);
        auto f = makeRDelegate(ovalue, &OValue::add);
        printf("Delegate: %d\n", sizeof(f));

        i32 val = 0;
        {
            VORB_SAMPLE_SCOPE(context);
            for (size_t i = 0; i < 15000000; i++) {
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
            }
        }
        std::cout << val << " Accum. Time: " << context.getAccumulatedSeconds() << std::endl;
    }

    {
        vorb::MTDetailedSamplerContext context;
        OValue ovalue(3);
        std::function<i32(i32,i32)> f = std::bind(&OValue::add, &ovalue, std::placeholders::_1, std::placeholders::_2);
        printf("Function: %d\n", sizeof(f));

        i32 val = 0;
        {
            VORB_SAMPLE_SCOPE(context);
            for (size_t i = 0; i < 15000000; i++) {
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
                val += f(val, val);
            }
        }
        std::cout << val << " Accum. Time: " << context.getAccumulatedSeconds() << std::endl;
    }

    return true;
}

TEST(Endianness) {
    ui16 v1 = 1;
    ui32 v2 = 1;
    ui64 v3 = 1;
    f32 v4 = 1.0f;
    v1 = vorb::endianSwap(v1);
    v2 = vorb::endianSwap(v2);
    v3 = vorb::endianSwap(v3);
    v4 = vorb::endianSwap(v4);
    return true;
}

#include <deps/include/SDL/SDL.h>
#include <deps/include/SDL/SDL_joystick.h>

TEST(Rumble) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_EVENTS);

    //Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
    }

    //Check for joysticks
    if (SDL_NumJoysticks() < 1) {
        printf("Warning: No joysticks connected!\n");
    } else {
        printf("Num Joysticks: %d\n", SDL_NumJoysticks());
        SDL_Joystick* gGameController;

        for (int i = SDL_NumJoysticks(); i--;) {
            puts("\n\nNew joystick");
            //Load joystick
            gGameController = SDL_JoystickOpen(0);
            if (!gGameController) {
                printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
                continue;
            }
            printf("Name: %s\n", SDL_JoystickName(gGameController));

            for (int j = SDL_JoystickNumAxes(gGameController); j--;) {
                printf("Axis: %d\n", SDL_JoystickGetAxis(gGameController, j));
            }


            SDL_Haptic* haptic = SDL_HapticOpenFromJoystick(gGameController);
            if (haptic) {
                puts("Detected haptic feedback");
                SDL_HapticRumbleInit(haptic);
                SDL_HapticRumblePlay(haptic, 1, 1000);
                SDL_Delay(1500);
            }
        }

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);

        while (true) {
            SDL_Event e;
            while(SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_QUIT:
                    return true;
                default:
                    break;
                }
            }

            if (system("CLS")) system("clear");
            for (int j = SDL_JoystickNumAxes(gGameController); j--;) {
                printf("Axis: %d\n", SDL_JoystickGetAxis(gGameController, j));
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(100);
        }
    }


    return true;
}
