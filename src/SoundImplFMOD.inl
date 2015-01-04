///
/// SoundImplFMOD.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef SoundImplFMOD_h__
#define SoundImplFMOD_h__

#include <fmod/fmod.hpp>

namespace vorb {
    namespace sound {
        namespace impl {
            struct ResourceData {
            public:
                FMOD::Sound* sound = nullptr;
            };

            struct InstanceData {
            public:
                FMOD::Channel* channel = nullptr;
            };
        }
    }
}

#endif // SoundImplFMOD_h__