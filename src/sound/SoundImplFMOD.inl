///
/// SoundImplFMOD.inl
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// FMOD sound data implementation
///

#pragma once

#ifndef SoundImplFMOD_inl__
#define SoundImplFMOD_inl__

#include <fmod/fmod.hpp>

#define VORB_SOUND_ENGINE_MAX_CHANNELS 512

namespace vorb {
    namespace sound {
        namespace impl {
            struct InstanceData {
            public:
                FMOD::Channel* channel = nullptr;
            };

            struct EngineData {
            public:
                FMOD::System* system = nullptr;
                ui32 currentChannel = 0;
                InstanceData channels[VORB_SOUND_ENGINE_MAX_CHANNELS];
            };

            struct ResourceData {
            public:
                FMOD::Sound* sound = nullptr;
            };
        }
    }
}

#endif // SoundImplFMOD_inl__