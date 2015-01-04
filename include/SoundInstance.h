///
/// SoundInstance.h
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

#ifndef SoundInstance_h__
#define SoundInstance_h__

#include "SoundImpl.h"

namespace vorb {
    namespace sound {
        class Instance {
            friend class Engine;
        public:
            Instance();

            void play();

            bool isPlaying() const;
        private:
            Instance(Engine& engine);

            Engine* m_engine = nullptr;
            impl::InstanceData* m_data = nullptr;
        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundInstance_h__
