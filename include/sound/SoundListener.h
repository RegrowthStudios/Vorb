///
/// SoundListener.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Positional information for a listener to a sound engine
///

#pragma once

#ifndef SoundListener_h__
#define SoundListener_h__

namespace vorb {
    namespace sound {
        /// A listener located in 3D "sound" space
        class Listener {
        public:
            UNIT_SPACE(SOUND) f32v3 position { 0, 0, 0 }; ///< Listener's position in "sound" space
            UNIT_SPACE(SOUND) f32v3 forward { 0, 0, -1 }; ///< Forward-facing direction
            UNIT_SPACE(SOUND) f32v3 up { 0, 1, 0 }; ///< Upwards vector
            UNIT_SPACE(SOUND) f32v3 velocity { 0, 0, 0 }; ///< Current velocity (for advanced effects)
        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundListener_h__
