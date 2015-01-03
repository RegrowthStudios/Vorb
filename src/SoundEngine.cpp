#include "stdafx.h"
#include "SoundEngine.h"

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#define VORB_SOUND_SYSTEM_MAX_CHANNELS 512

namespace vorb {
    namespace sound {
        namespace impl {
            FMOD::System* system = nullptr; ///< Sound system
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
