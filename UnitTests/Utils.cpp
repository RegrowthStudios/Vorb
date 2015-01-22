#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Utils_

#include <random>
#include <thread>

#include <include/Random.h>
#include <include/RingBuffer.hpp>
#include <include/Timing.h>
#include <include/ScopedTiming.hpp>

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