//
// SoundInstance.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file SoundInstance.h
 * @brief An instance of a sound - the active part that is played
 */

#pragma once

#ifndef Vorb_SoundInstance_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SoundInstance_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

// TODO: Is it possible to cache results?
// TODO: Use relative frequency?

#include "ISoundImpl.h"
#include "SoundEngine.h"

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
            /// Set this instance's looping information
            /// @param loop: True if sound should looping
            void setLooped(const bool& loop);
            /// Move this instance's playback cursor
            /// @param seconds: Time in seconds
            void setCursor(const f32& seconds);
            /// Move the instance to a certain position
            /// @param pos: 3D position
            void setPosition(UNIT_SPACE(SOUND) const f32v3& pos);
            /// Set the instance's velocity
            /// @param vel: 3D velocity
            void setVelocity(UNIT_SPACE(SOUND) const f32v3& vel);
            /// Set sound falloff levels
            /// @param min: Distance from sound at which attenuation takes effect
            /// @param max: Distance from sound at which attenuation stops and is not heard anymore
            void setBoundsRadii(UNIT_SPACE(SOUND) const f32& min, UNIT_SPACE(SOUND) const f32& max);
            /// Set the volume (maximum loudness) of the sound
            /// @param v: Percentage of resource volume level [0,1]
            void setVolume(const f32& v);
            /// Set the sound's sampling frequency for playback
            /// @param freq: Sampling frequency (in Hertz)
            void setFrequency(const f32& freq);

            /// @return True if this is playing
            bool isPlaying() const;
            /// @return True if this is a looping sound
            bool isLooped() const;
            /// @return The length of this sound instance (in seconds)
            f32 getLength() const;
            /// @return The 3D position of the sound
            UNIT_SPACE(SOUND) f32v3 getPosition() const;
            /// @return The sound's velocity
            UNIT_SPACE(SOUND) f32v3 getVelocity() const;
            /// @return The radii for which sound attenuation is present
            UNIT_SPACE(SOUND) f32v2 getBoundsRadii() const;
            /// @return The volume of the sound (typically [0,1])
            f32 getVolume() const;
            /// @return The frequency of the sound (in Hertz)
            f32 getFrequency() const;
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

#endif // !Vorb_SoundInstance_h__
