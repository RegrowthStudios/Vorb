///
/// SoundEngine.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// The main interface to keeping track of sounds
///

#pragma once

#ifndef SoundEngine_h__
#define SoundEngine_h__

namespace vorb {
    namespace sound {
        namespace impl {
            bool initSystem();
            bool disposeSystem();
        }
    }
}
namespace vsound = vorb::sound;

#endif // SoundEngine_h__
