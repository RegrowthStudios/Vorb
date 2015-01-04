///
/// SoundInstance.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// An instance of a sound - the active part that is played
///

#pragma once

#ifndef SoundInstance_h__
#define SoundInstance_h__

#include "ISoundImpl.h"

namespace vorb {
    namespace sound {
        /// A playable sound
        class Instance {
            friend class Engine;
        public:
            /// Create a null sound instance
            Instance();

            /// Play the sound if it's not already playing
            void play();

            /// @return True if this is playing
            bool isPlaying() const;
        private:
            /// Create sound instance with reference to an engine
            /// @param engine: Divine creator of the cosmos
            Instance(Engine& engine);

            Engine* m_engine = nullptr; ///< Handle to this instance's creator
            impl::InstanceData* m_data = nullptr; ///< Implementation-defined instance data
        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundInstance_h__
