#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Utils_

#include <include/Vorb.h>
#include <include/Timing.h>

TEST(TimingPrecision) {
    static const size_t TRIALS = 10;
    static const ui32 SLEEP_TIME = 1;

    vorb::HighPrecisionTime timeBegin, timeEnd;

    // Test with default OS precision
    vorb::getTimePrecise();
    puts("Low Precision");
    for (size_t i = 0; i < TRIALS; i++) {
        timeBegin = vorb::getTimePrecise();
        vorb::sleep(SLEEP_TIME);
        timeEnd = vorb::getTimePrecise();
        printf("Time Slept: %f\n", vorb::toSeconds(timeEnd - timeBegin));
    }

    // Test with better OS precision
    puts("\nHigh Precision");
    if (vorb::init(vorb::InitParam::TIMING) != vorb::InitParam::TIMING) return false;
    for (size_t i = 0; i < TRIALS; i++) {
        timeBegin = vorb::getTimePrecise();
        vorb::sleep(SLEEP_TIME);
        timeEnd = vorb::getTimePrecise();
        printf("Time Slept: %f\n", vorb::toSeconds(timeEnd - timeBegin));
    }
    if (vorb::dispose(vorb::InitParam::TIMING) != vorb::InitParam::TIMING) return false;

    return true;
}
