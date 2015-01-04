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

#include "Path.h"

namespace vorb {
    namespace sound {
        class Resource;
        class Instance;
        class Listener;

        namespace impl {
            bool initSystem();
            bool disposeSystem();
        }

        /// Manages many sound resources
        class Engine {
        public:
            Resource loadSound(const vio::Path& path);
            void freeSound(Resource& sound);

            Instance createSound(const Resource& sound);
            void update(const Listener& listener);
        private:

        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundEngine_h__
