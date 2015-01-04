#include "stdafx.h"
#include "SoundEngine.h"

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include "SoundInstance.h"
#include "SoundListener.h"
#include "SoundResource.h"

// TODO: Use FMOD #ifdefs
#include "SoundImplFMOD.inl"

#define VORB_SOUND_SYSTEM_MAX_CHANNELS 512

namespace vorb {
    namespace sound {
        namespace impl {
            FMOD::System* system = nullptr; ///< Sound system
            ui32 currentChannel = 0;
            impl::InstanceData channels[VORB_SOUND_SYSTEM_MAX_CHANNELS] = {};
        }
    }
}
bool vsound::impl::initSystem() {
    // Check for a previous system
    if (system) return false;

    FMOD_RESULT result;

    result = FMOD::System_Create(&system);      // Create the main system object.
    if (result != FMOD_OK) {
#ifdef DEBUG
        printf("FMOD Create Error:\n(%d) %s\n", result, FMOD_ErrorString(result));
#endif // DEBUG
        return false;
    }

    result = system->init(VORB_SOUND_SYSTEM_MAX_CHANNELS, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);    // Initialize FMOD.
    if (result != FMOD_OK) {
#ifdef DEBUG
        printf("FMOD Init Error:\n(%d) %s\n", result, FMOD_ErrorString(result));
#endif // DEBUG
        return false;
    }

    return true;
}
bool vsound::impl::disposeSystem() {
    if (!system) return true;

    FMOD_RESULT result;

    result = system->release();      // Create the main system object.
    if (result != FMOD_OK) {
#ifdef DEBUG
        printf("FMOD Release Error!\n(%d) %s\n", result, FMOD_ErrorString(result));
#endif // DEBUG
        return false;
    }

    system = nullptr;
    return true;
}

vsound::Resource vsound::Engine::loadSound(const vio::Path& path) {
    Resource sound;
    sound.m_data = new impl::ResourceData();
    impl::system->createSound(path.getCString(), FMOD_3D, nullptr, &sound.m_data->sound);
    return sound;
}
void vsound::Engine::freeSound(Resource& sound) {
    sound.m_data->sound->release();
    delete sound.m_data;
    sound.m_data = nullptr;
}
vsound::Instance vsound::Engine::createSound(const Resource& sound) {
    Instance inst;

    impl::currentChannel++;
    if (impl::currentChannel >= VORB_SOUND_SYSTEM_MAX_CHANNELS) impl::currentChannel = 0;

    inst.m_data = &impl::channels[impl::currentChannel];
    impl::system->playSound(FMOD_CHANNEL_FREE, sound.m_data->sound, true, &inst.m_data->channel);
    return inst;
}
void vsound::Engine::update(const Listener& listener) {
    impl::system->set3DListenerAttributes(0, // TODO: Use listener IDs 
        (FMOD_VECTOR*)&listener.position,
        (FMOD_VECTOR*)&listener.velocity,
        (FMOD_VECTOR*)&listener.forward,
        (FMOD_VECTOR*)&listener.up
        );

    impl::system->update();
}
