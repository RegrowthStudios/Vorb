#include "stdafx.h"
#include "Timing.h"

#ifndef VORB_USING_PCH
#include "compat.h"
#endif // !VORB_USING_PCH

#if defined(VORB_OS_WINDOWS)
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

typedef std::chrono::milliseconds ms;

const f64 MS_PER_SECOND = 1000.0;

void PreciseTimer::start() {
    m_timerRunning = true;
    m_start = std::chrono::high_resolution_clock::now();
}

// Returns time in ms
f64 PreciseTimer::stop() {
    m_timerRunning = false;
    std::chrono::duration<f64> duration = std::chrono::high_resolution_clock::now() - m_start;
    return duration.count() * MS_PER_SECOND;
}

void AccumulationTimer::start(const nString& tag) {
    if (m_timerRunning) stop();
    m_timerRunning = true;
    m_start = std::chrono::high_resolution_clock::now();
    m_it = m_accum.find(tag);
    if (m_it == m_accum.end()) {
        m_it = m_accum.insert(std::make_pair(tag, AccumNode())).first;
    }
}

f64 AccumulationTimer::stop() {
    m_timerRunning = false;
    std::chrono::duration<f64> duration = std::chrono::high_resolution_clock::now() - m_start;
    f64 time = duration.count() * MS_PER_SECOND;
    m_it->second.addSample(time);
    return time;
}

void AccumulationTimer::clear() {
    m_accum.clear();
    m_timerRunning = false;
}

void AccumulationTimer::printAll(bool averages) {
    if (averages) {
        for (auto& it : m_accum) {
            printf("  %-20s: %12lf ms\n", it.first.c_str(), (it.second.time / (f64)it.second.numSamples));
        }
    } else {
        for (auto& it : m_accum) {
            printf("  %-20s: %12lf ms\n", it.first.c_str(), it.second.time / 10.0f);
        }
    }
}

void MultiplePreciseTimer::start(const nString& tag) {
    if (m_timer.isRunning()) stop();
    if (m_index >= m_intervals.size()) {
        m_intervals.push_back(Interval(tag));
    } else {
        m_intervals[m_index].tag = tag;
    }
    m_timer.start();

}
void MultiplePreciseTimer::stop() {
    m_intervals[m_index++].time += m_timer.stop();
}
void MultiplePreciseTimer::end(const bool& print) {
    if (m_timer.isRunning()) m_timer.stop();
    if (m_intervals.empty()) return;
    if (m_samples == m_desiredSamples) {
        if (print) {
            printf("TIMINGS: \n");
            for (size_t i = 0; i < m_intervals.size(); i++) {
                printf("  %-20s: %12lf ms\n", m_intervals[i].tag.c_str(), m_intervals[i].time / m_samples);
            }
            printf("\n");
        }
        m_intervals.clear();
        m_samples = 0;
        m_index = 0;
    } else {
        m_index = 0;
        m_samples++;
    }
}

void FpsCounter::beginFrame() {
    m_startTicks = SDL_GetTicks();
}
f32 FpsCounter::endFrame() {
    calculateFPS();
    return m_fps;
}
void FpsCounter::calculateFPS() {

    #define DECAY 0.9f

    //Calculate the number of ticks (ms) for this frame
    f32 timeThisFrame = (f32)(SDL_GetTicks() - m_startTicks);
    // Use a simple moving average to decay the FPS
    m_frameTime = m_frameTime * DECAY + timeThisFrame * (1.0f - DECAY);

    //Calculate FPS
    if (m_frameTime > 0.0f) {
        m_fps = (f32)(MS_PER_SECOND / m_frameTime);
    } else {
        m_fps = 60.0f;
    }
}

void FpsLimiter::init(const f32& maxFPS) {
    setMaxFPS(maxFPS);
}
void FpsLimiter::setMaxFPS(const f32& maxFPS) {
    m_maxFPS = maxFPS;
}
f32 FpsLimiter::endFrame() {
    calculateFPS();

    f32 frameTicks = (f32)(SDL_GetTicks() - m_startTicks);
    //Limit the FPS to the max FPS
    if (MS_PER_SECOND / m_maxFPS > frameTicks) {
        SDL_Delay((ui32)(MS_PER_SECOND / m_maxFPS - frameTicks));
    }

    return m_fps;
}