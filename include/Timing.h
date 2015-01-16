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
    /// A timer that samples a time period via its lifetime
    /// @tparam T Context type that stores sample information (must overload T& operator+= (ui64 microSeconds))
    template<typename T = ui64>
    class ScopedSampler {
    public:
        /// Hook the timer up to an output and take the starting time
        /// @param context: Destination for time data
        ScopedSampler(OUT T& context) :
            m_context(context) {
            m_time = std::chrono::high_resolution_clock::now();
        }
        /// Hook the timer up to an output and take the starting time
        /// @param context: Destination for time data
        ScopedSampler(OUT T* context) : ScopedSampler(*context) {
            // Empty
        }
        /// Finish keeping track of time (add time sample to context)
        ~ScopedSampler() {
            auto difference = std::chrono::high_resolution_clock::now() - m_time;
            m_context += std::chrono::duration_cast<std::chrono::microseconds>(difference).count();
        }
    private:
        T& m_context; ///< Timing information destination
        std::chrono::system_clock::time_point m_time {}; ///< Start time
    };

    /// Accumulates time slices
    class AccumulationSamplerContext {
    public:
        /// Add elapsed milliseconds to this context
        /// @param dt: Elapsed time
        /// @return Self
        AccumulationSamplerContext& operator+=(UNIT_SPACE(MICROSECONDS) const ui64& dt) {
            m_ticks += dt;
            return *this;
        }

        const ui64& getAccumulatedMicroseconds() const {
            return m_ticks;
        }
        const f64 getAccumulatedMilliSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_MILLISECOND_F64;
        }
        const f64 getAccumulatedSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_SECOND_F64;
        }
    protected:
        UNIT_SPACE(MICROSECONDS) ui64 m_ticks = 0; ///< Accumulated time
    };
    typedef ScopedSampler<AccumulationSamplerContext> ScopedAccumulationSampler;

    /// Accumulates time slices and number of time slices taken
    class AveragedSamplerContext {
    public:
        /// Add elapsed milliseconds to this context and increment number of slices
        /// @param dt: Elapsed time
        /// @return Self
        AveragedSamplerContext& operator+=(UNIT_SPACE(MICROSECONDS) const ui64& dt) {
            m_ticks += dt;
            m_entries++;
            return *this;
        }

        const ui64& getAccumulatedMicroseconds() const {
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
        const ui64 getAverageMicroseconds() const {
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
        UNIT_SPACE(MICROSECONDS) ui64 m_ticks = 0; ///< Accumulated time
        ui64 m_entries = 0; ///< Count of times context was incremented
    };
    typedef ScopedSampler<AveragedSamplerContext> ScopedAveragedSampler;

    /// Accumulates time slices, number of time slices taken, and min/max time slices
    class DetailedSamplerContext {
    public:
        /// Add elapsed milliseconds to this context, increment slices, and check bounds
        /// @param dt: Elapsed time
        /// @return Self
        DetailedSamplerContext& operator+=(UNIT_SPACE(MICROSECONDS) const ui64& dt) {
            m_ticks += dt;
            m_entries++;
            if (dt < m_bounds.x) m_bounds.x = dt;
            if (dt > m_bounds.y) m_bounds.y = dt;
            return *this;
        }

        const ui64& getAccumulatedMicroseconds() const {
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
        const ui64 getAverageMicroseconds() const {
            ui64 rounded = m_ticks + (m_entries >> 1);
            return rounded / m_entries;
        }
        const f64 getAverageMilliseconds() const {
            return getAccumulatedMilliSeconds() / (f64)m_entries;
        }
        const f64 getAverageSeconds() const {
            return getAccumulatedSeconds() / (f64)m_entries;
        }

        const ui64& getMinElapsedMicroseconds() const {
            return m_bounds.x;
        }
        const ui64& getMaxElapsedMicroseconds() const {
            return m_bounds.y;
        }
    protected:
        UNIT_SPACE(MICROSECONDS) ui64 m_ticks = 0; ///< Accumulated time
        ui64 m_entries = 0; ///< Count of times context was incremented
        ui64v2 m_bounds { ~0ui64 , 0 }; ///< Min/max delta times
    };
    typedef ScopedSampler<DetailedSamplerContext> ScopedDetailedSampler;
}

#endif // Timing_h__