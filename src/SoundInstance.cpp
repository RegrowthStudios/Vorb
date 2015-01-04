#include "stdafx.h"
#include "SoundInstance.h"

// TODO: Add FMOD #ifdefs
#include "SoundImplFMOD.inl"

vorb::sound::Instance::Instance() {
    // Empty
}

vorb::sound::Instance::Instance(Engine& engine) :
    m_engine(&engine) {
    // Empty
}

void vorb::sound::Instance::play() {
    m_data->channel->setPaused(false);
}

bool vorb::sound::Instance::isPlaying() const {
    bool ret;
    m_data->channel->isPlaying(&ret);
    return ret;
}
