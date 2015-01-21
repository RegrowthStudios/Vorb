///
/// ScopedTiming.hpp
/// Vorb Engine
///
/// Created by Cristian Zaloj on 16 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Simple implementation of an lifetime-based timer
///

#pragma once

#ifndef ScopedTiming_hpp__
#define ScopedTiming_hpp__

#include <atomic>
#include <chrono>

#include "Constants.h"

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
        f64 getAccumulatedMilliSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_MILLISECOND_F64;
        }
        f64 getAccumulatedSeconds() const {
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
        f64 getAccumulatedMilliSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_MILLISECOND_F64;
        }
        f64 getAccumulatedSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_SECOND_F64;
        }

        const ui64& getEntryCount() const {
            return m_entries;
        }
        ui64 getAverageMicroseconds() const {
            ui64 rounded = m_ticks + (m_entries >> 1);
            return rounded / m_entries;
        }
        f64 getAverageMilliseconds() const {
            return getAccumulatedMilliSeconds() / (f64)m_entries;
        }
        f64 getAverageSeconds() const {
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
        f64 getAccumulatedMilliSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_MILLISECOND_F64;
        }
        f64 getAccumulatedSeconds() const {
            return (f64)m_ticks / MICROSECONDS_PER_SECOND_F64;
        }

        const ui64& getEntryCount() const {
            return m_entries;
        }
        ui64 getAverageMicroseconds() const {
            ui64 rounded = m_ticks + (m_entries >> 1);
            return rounded / m_entries;
        }
        f64 getAverageMilliseconds() const {
            return getAccumulatedMilliSeconds() / (f64)m_entries;
        }
        f64 getAverageSeconds() const {
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
        UNIT_SPACE(MICROSECONDS) ui64v2 m_bounds { ~0ui64, 0 }; ///< Min/max delta times
    };
    typedef ScopedSampler<DetailedSamplerContext> ScopedDetailedSampler;

    /// Accumulates time slices (thread-safe)
    class MTAccumulationSamplerContext {
    public:
        /// Add elapsed milliseconds to this context
        /// @param dt: Elapsed time
        /// @return Self
        MTAccumulationSamplerContext& operator+=(UNIT_SPACE(MICROSECONDS) const ui64& dt) {
            std::atomic_fetch_add(&m_ticks, dt);
            return *this;
        }

        ui64 getAccumulatedMicroseconds() const {
            return std::atomic_load(&m_ticks);
        }
        f64 getAccumulatedMilliSeconds() const {
            return (f64)std::atomic_load(&m_ticks) / MICROSECONDS_PER_MILLISECOND_F64;
        }
        f64 getAccumulatedSeconds() const {
            return (f64)std::atomic_load(&m_ticks) / MICROSECONDS_PER_SECOND_F64;
        }
    protected:
        UNIT_SPACE(MICROSECONDS) std::atomic<ui64> m_ticks = ATOMIC_VAR_INIT(0); ///< Accumulated time
    };
    typedef ScopedSampler<MTAccumulationSamplerContext> MTScopedAccumulationSampler;

    /// Accumulates time slices and number of time slices taken (thread-safe)
    class MTAveragedSamplerContext {
    public:
        /// Add elapsed milliseconds to this context and increment number of slices
        /// @param dt: Elapsed time
        /// @return Self
        MTAveragedSamplerContext& operator+=(UNIT_SPACE(MICROSECONDS) const ui64& dt) {
            std::atomic_fetch_add(&m_ticks, dt);
            std::atomic_fetch_add(&m_entries, 1);
            return *this;
        }

        ui64 getAccumulatedMicroseconds() const {
            return std::atomic_load(&m_ticks);
        }
        f64 getAccumulatedMilliSeconds() const {
            return (f64)std::atomic_load(&m_ticks) / MICROSECONDS_PER_MILLISECOND_F64;
        }
        f64 getAccumulatedSeconds() const {
            return (f64)std::atomic_load(&m_ticks) / MICROSECONDS_PER_SECOND_F64;
        }

        ui64 getEntryCount() const {
            return std::atomic_load(&m_entries);
        }
        ui64 getAverageMicroseconds() const {
            ui64 entries = std::atomic_load(&m_entries);
            ui64 ticks = std::atomic_load(&m_ticks);

            ui64 rounded = ticks + (entries >> 1);
            return rounded / entries;
        }
        f64 getAverageMilliseconds() const {
            ui64 entries = std::atomic_load(&m_entries);
            ui64 ticks = std::atomic_load(&m_ticks);

            return ((f64)ticks / MICROSECONDS_PER_MILLISECOND_F64) / (f64)entries;
        }
        f64 getAverageSeconds() const {
            ui64 entries = std::atomic_load(&m_entries);
            ui64 ticks = std::atomic_load(&m_ticks);

            return ((f64)ticks / MICROSECONDS_PER_SECOND_F64) / (f64)entries;
        }
    protected:
        UNIT_SPACE(MICROSECONDS) std::atomic<ui64> m_ticks = ATOMIC_VAR_INIT(0); ///< Accumulated time
        std::atomic<ui64> m_entries = ATOMIC_VAR_INIT(0); ///< Count of times context was incremented
    };
    typedef ScopedSampler<MTAveragedSamplerContext> MTScopedAveragedSampler;

    /// Accumulates time slices, number of time slices taken, and min/max time slices (thread-safe)
    class MTDetailedSamplerContext {
    public:
        /// Add elapsed milliseconds to this context, increment slices, and check bounds
        /// @param dt: Elapsed time
        /// @return Self
        MTDetailedSamplerContext& operator+=(UNIT_SPACE(MICROSECONDS) const ui64& dt) {
            std::atomic_fetch_add(&m_ticks, dt);
            std::atomic_fetch_add(&m_entries, 1);
            ui64 cmp = std::atomic_load(&m_minTime);
            while (dt < cmp) std::atomic_compare_exchange_weak(&m_minTime, &cmp, dt);
            cmp = std::atomic_load(&m_maxTime);
            while (dt > cmp) std::atomic_compare_exchange_weak(&m_maxTime, &cmp, dt);
            return *this;
        }

        ui64 getAccumulatedMicroseconds() const {
            return std::atomic_load(&m_ticks);
        }
        f64 getAccumulatedMilliSeconds() const {
            return (f64)std::atomic_load(&m_ticks) / MICROSECONDS_PER_MILLISECOND_F64;
        }
        f64 getAccumulatedSeconds() const {
            return (f64)std::atomic_load(&m_ticks) / MICROSECONDS_PER_SECOND_F64;
        }

        ui64 getEntryCount() const {
            return std::atomic_load(&m_entries);
        }
        ui64 getAverageMicroseconds() const {
            ui64 entries = std::atomic_load(&m_entries);
            ui64 ticks = std::atomic_load(&m_ticks);

            ui64 rounded = ticks + (entries >> 1);
            return rounded / entries;
        }
        f64 getAverageMilliseconds() const {
            ui64 entries = std::atomic_load(&m_entries);
            ui64 ticks = std::atomic_load(&m_ticks);

            return ((f64)ticks / MICROSECONDS_PER_MILLISECOND_F64) / (f64)entries;
        }
        f64 getAverageSeconds() const {
            ui64 entries = std::atomic_load(&m_entries);
            ui64 ticks = std::atomic_load(&m_ticks);

            return ((f64)ticks / MICROSECONDS_PER_SECOND_F64) / (f64)entries;
        }

        ui64 getMinElapsedMicroseconds() const {
            return std::atomic_load(&m_minTime);
        }
        ui64 getMaxElapsedMicroseconds() const {
            return std::atomic_load(&m_maxTime);
        }
    protected:
        UNIT_SPACE(MICROSECONDS) std::atomic<ui64> m_ticks = ATOMIC_VAR_INIT(0); ///< Accumulated time
        std::atomic<ui64> m_entries = ATOMIC_VAR_INIT(0); ///< Count of times context was incremented
        UNIT_SPACE(MICROSECONDS) std::atomic<ui64> m_minTime = ATOMIC_VAR_INIT(~0ui64); ///< Min delta time
        UNIT_SPACE(MICROSECONDS) std::atomic<ui64> m_maxTime = ATOMIC_VAR_INIT(0); ///< Max delta time
    };
    typedef ScopedSampler<MTDetailedSamplerContext> MTScopedDetailedSampler;

    template<typename T>
    ScopedSampler<T> getInferredScopedSampler(T& obj) {
        return ScopedSampler<T>(obj);
    }
}

#define VORB_COMBINE1(X,Y) X##Y  // Combines macros
#define VORB_COMBINE(X,Y) VORB_COMBINE1(X,Y)
#define VORB_SAMPLE_SCOPE(VALUE) auto VORB_COMBINE(__vorb_scopedTimer_ , __LINE__) = vorb::getInferredScopedSampler(VALUE);


#endif // ScopedTiming_hpp__
