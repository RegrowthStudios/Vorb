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

#include "Constants.h"

class PreciseTimer {
public:
    void start();
    f32 stop();

    const bool& isRunning() const {
        return m_timerRunning;
    }
private:
    bool m_timerRunning = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
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
        f32 time = 0.0f;
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

namespace vorb {
    template<typename T = ui64>
    class ScopedSampler {
    public:
        ScopedSampler(T& context) :
            m_context(context) {
            m_time = std::chrono::high_resolution_clock::now();
        }
        ScopedSampler(T* context) : ScopedSampler(*context) {
            // Empty
        }
        ~ScopedSampler() {
            auto difference = std::chrono::high_resolution_clock::now() - m_time;
            m_context += std::chrono::duration_cast<std::chrono::microseconds>(difference).count();
        }
    private:
        T& m_context;
        std::chrono::system_clock::time_point m_time {};
    };

    class AccumulationSamplerContext {
    public:
        AccumulationSamplerContext& operator+=(const ui64& elapsedTicks) {
            m_ticks += elapsedTicks;
            return *this;
        }

        const ui64& getAccumulatedTicks() const {
            return m_ticks;
        }
        const f64 getAccumulatedMilliSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_MILLISECOND_F64;
        }
        const f64 getAccumulatedSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_SECOND_F64;
        }
    protected:
        ui64 m_ticks = 0;
    };
    typedef ScopedSampler<AccumulationSamplerContext> ScopedAccumulationSampler;

    class AveragedSamplerContext {
    public:
        AveragedSamplerContext& operator+=(const ui64& elapsedTicks) {
            m_ticks += elapsedTicks;
            m_entries++;
            return *this;
        }

        const ui64& getAccumulatedTicks() const {
            return m_ticks;
        }
        const f64 getAccumulatedMilliSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_MILLISECOND_F64;
        }
        const f64 getAccumulatedSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_SECOND_F64;
        }

        const ui64& getEntryCount() const {
            return m_ticks;
        }
        const ui64 getAverageTicks() const {
            ui64 rounded = m_ticks + (m_entries >> 1);
            return rounded / m_entries;
        }
        const f64 getAverageMilliseconds() const {
            return getAccumulatedMilliSeconds() / (f64)m_entries;
        }
        const f64 getAverageSeconds() const {
            return getAccumulatedSeconds() / (f64)m_entries;
        }
    protected:
        ui64 m_ticks = 0;
        ui64 m_entries = 0;
    };
    typedef ScopedSampler<AveragedSamplerContext> ScopedAveragedSampler;

    class DetailedSamplerContext {
    public:
        DetailedSamplerContext& operator+=(const ui64& elapsedTicks) {
            m_ticks += elapsedTicks;
            m_entries++;
            if (elapsedTicks < m_bounds.x) m_bounds.x = elapsedTicks;
            if (elapsedTicks > m_bounds.y) m_bounds.y = elapsedTicks;
            return *this;
        }

        const ui64& getAccumulatedTicks() const {
            return m_ticks;
        }
        const f64 getAccumulatedMilliSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_MILLISECOND_F64;
        }
        const f64 getAccumulatedSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_SECOND_F64;
        }

        const ui64& getEntryCount() const {
            return m_ticks;
        }
        const ui64 getAverageTicks() const {
            ui64 rounded = m_ticks + (m_entries >> 1);
            return rounded / m_entries;
        }
        const f64 getAverageMilliseconds() const {
            return getAccumulatedMilliSeconds() / (f64)m_entries;
        }
        const f64 getAverageSeconds() const {
            return getAccumulatedSeconds() / (f64)m_entries;
        }

        const ui64& getMinTicks() const {
            return m_bounds.x;
        }
        const ui64& getMaxTicks() const {
            return m_bounds.y;
        }
    protected:
        ui64 m_ticks = 0;
        ui64 m_entries = 0;
        ui64v2 m_bounds { 0ui64 - 1 , 0 };
    };
    typedef ScopedSampler<DetailedSamplerContext> ScopedDetailedSampler;
}

#endif // Timing_h__