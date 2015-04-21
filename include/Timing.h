///
/// Timing.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 8 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Different timers
///

#pragma once

#ifndef Timing_h__
#define Timing_h__

#include <chrono>

class PreciseTimer {
public:
    PreciseTimer() {
        start();
    }
    void start();
    f64 stop();

    const bool& isRunning() const {
        return m_timerRunning;
    }
private:
    bool m_timerRunning = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

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
    PreciseTimer m_timer;
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

#endif // Timing_h__
