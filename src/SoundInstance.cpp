#include "stdafx.h"
#include "SoundInstance.h"

// Include sound engine implementation details
#include "SoundImpl.hpp"

vsound::Instance::Instance() {
    // Empty
}
vsound::Instance::Instance(Engine& engine) :
    m_engine(&engine) {
    // Empty
}

#if defined(VORB_IMPL_SOUND_FMOD)
void vsound::Instance::play() {
    m_data->channel->setPaused(false);
}
void vsound::Instance::pause() {
    m_data->channel->setPaused(true);
}
void vsound::Instance::setLooped(const bool& loop) {
    m_data->channel->setLoopCount(loop ? -1 : 0);
}
void vsound::Instance::setCursor(const f32& seconds) {
    ui32 frames;
    FMOD::Sound* sound;
    m_data->channel->getCurrentSound(&sound);
    sound->getLength(&frames, FMOD_TIMEUNIT_MS);

    ui32 frame = (ui32)(seconds * 1000.0f);
    if (isLooped()) {
        frame %= frames;
    } else {
        if (frame > frames) frame = frames;
    }
    m_data->channel->setPosition(frame, FMOD_TIMEUNIT_MS);
}
void vsound::Instance::setPosition(const f32v3& pos) {
    m_data->channel->set3DAttributes((FMOD_VECTOR*)&pos, nullptr);
}
void vsound::Instance::setVelocity(const f32v3& vel) {
    m_data->channel->set3DAttributes(nullptr, (FMOD_VECTOR*)&vel);
}
void vsound::Instance::setBoundsRadii(const f32& min, const f32& max) {
    m_data->channel->set3DMinMaxDistance(min, max);
}
void vsound::Instance::setVolume(const f32& v) {
    m_data->channel->setVolume(v);
}
void vsound::Instance::setFrequency(const f32& freq) {
    m_data->channel->setFrequency(freq);
}

bool vsound::Instance::isPlaying() const {
    bool ret;
    m_data->channel->isPlaying(&ret);
    if (!ret) return false;
    m_data->channel->getPaused(&ret);
    return !ret;
}
bool vsound::Instance::isLooped() const {
    i32 loop;
    m_data->channel->getLoopCount(&loop);
    return loop < 0;
}
f32v3 vsound::Instance::getPosition() const {
    f32v3 pos, vel;
    m_data->channel->get3DAttributes(
        (FMOD_VECTOR*)&pos,
        (FMOD_VECTOR*)&vel
        );
    return pos;
}
f32v3 vsound::Instance::getVelocity() const {
    f32v3 pos, vel;
    m_data->channel->get3DAttributes(
        (FMOD_VECTOR*)&pos,
        (FMOD_VECTOR*)&vel
        );
    return vel;
}
f32v2 vsound::Instance::getBoundsRadii() const {
    f32v2 radii;
    m_data->channel->get3DMinMaxDistance(&radii.x, &radii.y);
    return radii;
}
f32 vsound::Instance::getVolume() const {
    f32 volume;
    m_data->channel->getVolume(&volume);
    return volume;
}
f32 vsound::Instance::getFrequency() const {
    f32 freq;
    m_data->channel->getFrequency(&freq);
    return freq;
}
f32 vsound::Instance::getLength() const {
    ui32 frames;
    FMOD::Sound* sound;
    m_data->channel->getCurrentSound(&sound);
    sound->getLength(&frames, FMOD_TIMEUNIT_MS);
    return (f32)frames / 1000.0f;
}
#endif // VORB_IMPL_SOUND