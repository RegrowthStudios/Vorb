#include "Vorb/stdafx.h"
#include "Vorb/sound/SoundEngine.h"

#include "Vorb/IDGenerator.h"
#include "Vorb/sound/SoundInstance.h"
#include "Vorb/sound/SoundListener.h"
#include "Vorb/sound/SoundResource.h"

// Include sound engine implementation details
#include "Vorb/sound/SoundImpl.hpp"

#include <mutex>

namespace vorb {
    namespace sound {
        namespace impl {
            // A list of initialized sound engines
            std::vector<vsound::Engine> engines;

            // Unique ID generator for resources
            vsound::ResourceIDGenerator idGenResources;
            std::mutex lckIDGenResources;
        }
    }
}
bool vsound::impl::initSystem() {
    idGenResources.reset();
    std::vector<vsound::Engine>().swap(engines);
    return true;
}
bool vsound::impl::disposeSystem() {
    // Attempt to dispose of all systems
    for (auto& engine : engines) {
        if (engine.isDisposed()) continue;
        engine.dispose();
    }
    std::vector<vsound::Engine>().swap(engines);
    return true;
}

#if defined(VORB_IMPL_SOUND_FMOD)
//#include <fmod/fmod.hpp>
//#include <fmod/fmod_errors.h>

bool vsound::Engine::init() {
    if (!isDisposed()) return true;

    // Allocate new data
    m_data = new impl::EngineData;

//    FMOD_RESULT result = FMOD::System_Create(&m_data->system);      // Create the main system object.
//    if (result != FMOD_OK) {
//#ifdef DEBUG
//        printf("FMOD Create Error:\n(%d) %s\n", result, FMOD_ErrorString(result));
//#endif // DEBUG
//        delete m_data;
//        return false;
//    }
//
//    result = m_data->system->init(VORB_SOUND_ENGINE_MAX_CHANNELS, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);    // Initialize FMOD.
//    if (result != FMOD_OK) {
//#ifdef DEBUG
//        printf("FMOD Init Error:\n(%d) %s\n", result, FMOD_ErrorString(result));
//#endif // DEBUG
//        m_data->system->release();
//        delete m_data;
//        return false;
//    }

    // Update liveliness
    m_alive.reset(new bool(true));
    impl::engines.push_back(*this);
    return true;
}
bool vsound::Engine::dispose() {
    if (isDisposed()) return true;

    // Dispose all managed sounds
    for (auto& kvp : m_resources) {
        disposeSound(kvp.second);
    }
    m_resources.clear();

//    m_data->system->close();
//    FMOD_RESULT result = m_data->system->release();      // Delete the main system object.
//    if (result != FMOD_OK) {
//#ifdef DEBUG
//        printf("FMOD Release Error!\n(%d) %s\n", result, FMOD_ErrorString(result));
//#endif // DEBUG
//        return false;
//    }

//    m_data->system = nullptr;
    delete m_data;
    m_data = nullptr;

    // Signal death
    *m_alive = false;
    return true;
}

vsound::Resource vsound::Engine::loadSound(const vio::Path& path, bool is3D /*= true*/, bool isStream /*= false*/) {
    Resource sound;
    ResourceID id;

    // Obtain ID
    sound.m_data = new impl::ResourceData();
    { std::unique_lock<std::mutex> lck(impl::lckIDGenResources);
        id = impl::idGenResources.generate();
    }

    // Add sound to managed list
    sound.m_id = id;
    m_resources[id] = sound;

    // Create sound data
//    FMOD_MODE mode = FMOD_LOOP_NORMAL;
//    mode |= is3D ? FMOD_3D : FMOD_2D;
//    mode |= isStream ? FMOD_CREATESTREAM : FMOD_CREATESAMPLE;
//    m_data->system->createSound(path.getCString(), mode, nullptr, &sound.m_data->sound);
//    sound.m_data->sound->setLoopCount(1);
    return sound;
}
void vsound::Engine::disposeSound(Resource& sound) {
    // Recycle ID
    { std::unique_lock<std::mutex> lck(impl::lckIDGenResources);
        impl::idGenResources.recycle(sound.m_id);
    }
    sound.m_id = ID_GENERATOR_NULL_ID;

    // Dispose sound data
//    auto res = sound.m_data->sound->release();
    delete sound.m_data;
    sound.m_data = nullptr;
}

vsound::Instance vsound::Engine::createInstance(const Resource& sound) {
    Instance inst;

//    m_data->currentChannel++;
//    if (m_data->currentChannel >= VORB_SOUND_ENGINE_MAX_CHANNELS) m_data->currentChannel = 0;
//
//    inst.m_data = &m_data->channels[m_data->currentChannel];
//    #if defined(VORB_OS_WINDOWS)
//    m_data->system->playSound(FMOD_CHANNEL_FREE, sound.m_data->sound, true, &inst.m_data->channel);
//    #else
//    m_data->system->playSound(sound.m_data->sound, nullptr, true, &inst.m_data->channel);
//    #endif
    return inst;
}

void vsound::Engine::update(const Listener& listener) {
//    m_data->system->set3DListenerAttributes(0, // TODO: Use listener IDs
//        (FMOD_VECTOR*)&listener.position,
//        (FMOD_VECTOR*)&listener.velocity,
//        (FMOD_VECTOR*)&listener.forward,
//        (FMOD_VECTOR*)&listener.up
//        );

//    m_data->system->update();
}
#endif // VORB_IMPL_SOUND

void vsound::Engine::freeSound(Resource& sound) {
    // Check if this engine manages the sound
    if (!containsSound(sound)) return;

    // Remove sound
    m_resources.erase(sound.m_id);

    disposeSound(sound);
}

bool vsound::Engine::containsSound(Resource& sound) const {
    if (sound.m_id == ID_GENERATOR_NULL_ID) return false;
    auto iter = m_resources.find(sound.m_id);
    return iter != m_resources.end();
}

vsound::Instance vsound::Engine::playInstance(const Resource& sound) {
    Instance inst = createInstance(sound);
    inst.play();
    return inst;
}
