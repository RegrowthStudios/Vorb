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
            /// Pause the sound it it's playing
            void pause();
            void setLooped(const bool& loop);
            void setCursor(const f32& seconds);
            void setPosition(const f32v3& pos);
            void setVelocity(const f32v3& vel);
            void setBoundsRadii(const f32& min, const f32& max);
            void setVolume(const f32& v);
            void setFrequency(const f32& freq);

            /// @return True if this is playing
            bool isPlaying() const;
            /// @return True if this is a looping sound
            bool isLooped() const;
            f32v3 getPosition() const;
            f32v3 getVelocity() const;
            f32v2 getBoundsRadii() const;
            f32 getVolume() const;
            f32 getFrequency() const;
            f32 getLength() const;
        private:
            /// Create sound instance with reference to an engine
            /// @param engine: Divine creator of the cosmos
            Instance(Engine& engine);

            Engine* m_engine = nullptr; ///< Handle to this instance's creator
            impl::InstanceData* m_data = nullptr; ///< Implementation-defined instance data

            // TODO: Is it possible to cache results?
        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundInstance_h__
