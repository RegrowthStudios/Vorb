///
/// SoundListener.h
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

#ifndef SoundListener_h__
#define SoundListener_h__

namespace vorb {
    namespace sound {
        class Listener {
        public:
            f32v3 position { 0, 0, 0 };
            f32v3 forward { 0, 0, -1 };
            f32v3 up { 0, 1, 0 };
            f32v3 velocity { 0, 0, 0 };
        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundListener_h__
