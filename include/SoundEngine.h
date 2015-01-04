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

#include "ISoundImpl.h"
#include "Path.h"
#include "SoundResource.h"

namespace vorb {
    namespace sound {
        class Instance;
        class Listener;

        namespace impl {
            // Initialize and dispose sound module
            VORB_INTERNAL bool initSystem();
            VORB_INTERNAL bool disposeSystem();
        }

        /// Manages many sound resources
        class Engine {
        public:
            /// Initialize this engine
            /// @return True if no error occurred and properly initialized
            bool init();
            /// Dispose this engine and any associated resources
            /// @return True if no error occurred and properly disposed
            bool dispose();

            /// @return True if this engine is currently initialized
            bool isDisposed() const {
                return (m_alive.get() == nullptr) || *m_alive;
            }

            /// Load a sound resource into this engine
            /// @param path: Path of sound
            /// @return Loaded sound resource (Null if could not load)
            Resource loadSound(const vio::Path& path);
            /// Destroy a sound resource loaded from this engine
            /// @param sound: Sound resource
            void freeSound(Resource& sound);

            /// Determines if this engine manages a sound resource
            /// @param sound: Sound resource
            /// @return True if this engine create this sound
            bool containsSound(Resource& sound) const;

            /// Create an active sound instance from a resource
            /// @pre: This engine contains the sound
            /// @param sound: Sound resource reference
            /// @return Handle to the paused sound instance
            Instance createInstance(const Resource& sound);
            /// Create an active sound instance from a resource and begin playing it
            /// @pre: This engine contains the sound
            /// @param sound: Sound resource reference
            /// @return Handle to the playing sound instance
            Instance playInstance(const Resource& sound);

            /// Update this sound engine and it's associated handles
            /// @param listener: The main sound listener
            void update(const Listener& listener);
        private:
            /// Internal sound resource disposal
            /// @param sound: Sound resource
            void disposeSound(Resource& sound);

            std::unordered_map<ResourceID, Resource> m_resources; ///< Sound resources managed by this engine
            std::shared_ptr<bool> m_alive; ///< Keeps track of liveliness of this engine instance
            impl::EngineData* m_data = nullptr; ///< Implementation-defined engine data
        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundEngine_h__
