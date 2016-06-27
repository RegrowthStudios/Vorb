//
// Timing.h
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Timing.h
 * @brief Different timers.
 */

#pragma once

#ifndef Vorb_Timing_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Timing_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>
#include <vector>

#include "types.h"
#endif // !VORB_USING_PCH

#include <chrono>

#if defined(VORB_OS_WINDOWS)
#define _WINSOCKAPI_
#include <Windows.h>
#endif

namespace vorb {
#if defined(VORB_OS_WINDOWS)
    typedef LARGE_INTEGER HighPrecisionTime;
#else
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> HighPrecisionTime;
#endif


    HighPrecisionTime getTimePrecise();

    f64 toSeconds(HighPrecisionTime time);

    /*!
     * \brief Sleeps the current thread.
     *
     * The amount of time the thread actually sleeps is really dependent on
     * the scheduler. To achieve better precision, make sure to initialize the timing
     * portion of Vorb.
     *
     * \param time The amount of time to sleep the thread in milliseconds.
     */
    void sleep(UNIT_SPACE(MS) ui32 time);

    class PreciseTimer {
    public:
        void start();
        f64 stop();

        const bool& isRunning() const {
            return m_timerRunning;
        }
    private:
        bool m_timerRunning;
        HighPrecisionTime m_start;
    };
}
vorb::HighPrecisionTime operator- (vorb::HighPrecisionTime lhs, vorb::HighPrecisionTime rhs);
vorb::HighPrecisionTime operator+ (vorb::HighPrecisionTime lhs, vorb::HighPrecisionTime rhs);


class AccumulationTimer {
public:
    void start(const nString& tag);
    f64 stop();

    void clear();
    void printAll(bool averages);
private:
    class AccumNode {
    public:
        void addSample(f64 sample) {
            numSamples++;
            time += sample;
        }
        i32 numSamples = 0;
        f64 time = 0.0f;
    };

    bool m_timerRunning = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    std::map<nString, AccumNode> m_accum;
    std::map<nString, AccumNode>::iterator m_it;
};

class MultiplePreciseTimer {
public:
    void start(const nString& tag);
    void stop();

    /// Prints all timings
    void end(const bool& print);
    void setDesiredSamples(const i32& desiredSamples) {
        m_desiredSamples = desiredSamples;
    }
private:
    struct Interval {
        Interval(const nString& Tag) :
            tag(Tag) {
            // Empty
        };
        
        nString tag;
        f64 time = 0.0f;
    };

    i32 m_samples = 0;
    i32 m_desiredSamples = 1;
    ui32 m_index = 0;
    vorb::PreciseTimer m_timer;
    std::vector<Interval> m_intervals;
};

/// Calculates FPS 
class FpsCounter {
public:
    /// Begins a frame timing for fps calculation
    void beginFrame();

    /// Ends the frame
    /// @return The current FPS
    f32 endFrame();

    /// Returns the current fps as last recorded by an endFrame
    const f32& getCurrentFps() const {
        return m_fps;
    }
protected:
    // Calculates the current FPS
    void calculateFPS();

    f32 m_fps = 0.0f;
    f32 m_frameTime = 0.0f;
    ui32 m_startTicks = 0;
};

#define DEFAULT_MAX_FPS 60.0f

/// Calculates FPS and also limits FPS
class FpsLimiter : public FpsCounter {
public:
    // Initializes the FPS limiter. For now, this is analogous to setMaxFPS
    void init(const f32& maxFPS);

    // Sets the desired max FPS
    void setMaxFPS(const f32& maxFPS);

    // Limits (blocks) the current frame to achieve the desired FPS
    /// @return Current FPS
    f32 endFrame();
private:
    f32 m_maxFPS = DEFAULT_MAX_FPS;
};

#endif // !Vorb_Timing_h__
