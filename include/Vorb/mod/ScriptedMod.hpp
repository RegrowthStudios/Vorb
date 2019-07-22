//
// ScriptedMod.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 16 July 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file ScriptedMod.h
 * \brief Holds all things needed for a given mod. This includes the mod's metadata, but also
 * basic utilities such as an IO manager, texture cache etc.
 */

#pragma once

#ifndef Vorb_ScriptedMod_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ScriptedMod_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Mod.h"
#include "ModIOManager.h"

namespace vorb {
    namespace mod {
        template <typename ScriptEnvironment>
        class ScriptedMod : public Mod {
        public:
            ScriptedMod();
            ~ScriptedMod() {
                // Empty.
            }

            // TODO(Matthew): We need to decide on the initialisation process. This is the entry point, so likely require something
            //                like "mod.yaml" which contains metadata, perhaps also specifies the mod's entry points for startup
            //                and shutdown? 
            /*!
             * \brief Initialises the mod, setting up necessary data, including loading
             * associated metadata.
             */
            virtual bool init(const nString& dir);
            /*!
             * \brief Disposes of the mod.
             */
            virtual void dispose();

            // TODO(Matthew): Determine start up and shut down processes for mods.
            //                    For scripted this will be a bit more obvious, but YAML this may still be TBD.
            /*!
             * \brief Starts up the mod.
             */
            virtual bool startup();
            /*!
             * \brief Shuts down the mod.
             */
            virtual bool shutdown();

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f);

            /*!
             * \brief Registers a script env builder to be used for building a mod's script environment.
             *
             * Note: These builders should be registered by subsystems in order for them to register constants, C callbacks
             * and so forth to the script environment.
             *
             * \param scriptEnvBuilder The script environment builder to be registered.
             */
            static void registerScriptEnvBuilder(Delegate<ScriptEnvironment*>&& scriptEnvBuilder);
        protected:
            /*!
             * \brief Builds the script environment.
             *
             * \param scriptEnv The script environment to build.
             */
            static void buildScriptEnv(ScriptEnvironment* scriptEnv);

            static std::vector<Delegate<ScriptEnvironment*>> scriptEnvBuilders; ///< The list of builders to use in building a script environment.

            ScriptEnvironment m_scriptEnv;
        };
    }
}
namespace vmod = vorb::mod;

template <typename ScriptEnvironment>
vmod::ScriptedMod<ScriptEnvironment>::ScriptedMod() :
    Mod() {
    // Empty.
}

template <typename ScriptEnvironment>
bool vmod::ScriptedMod<ScriptEnvironment>::init(const nString& dir) {
    if (!parent::init(dir)) return false;

    m_scriptEnv.init();
    
    // Pass script env to the builders. These will register all appropriate constants, C callbacks, etc. to the environment.
    buildScriptEnv(m_scriptEnv);

    // TODO(Matthew): Any other initialisation.

    return true;
}

template <typename ScriptEnvironment>
void vmod::ScriptedMod<ScriptEnvironment>::dispose() {
    parent::dispose();

    m_scriptEnv.dispose();
}

template <typename ScriptEnvironment>
void vmod::ScriptedMod<ScriptEnvironment>::registerScriptEnvBuilder(Delegate<ScriptEnvironment*>&& scriptEnvBuilder) {
    scriptEnvBuilders.emplace_back(std::forward<Delegate<ScriptEnvironment*>>(scriptEnvBuilder));
}

template <typename ScriptEnvironment>
void vmod::ScriptedMod<ScriptEnvironment>::buildScriptEnv(ScriptEnvironment* scriptEnv) {
    for  (auto& builder : scriptEnvBuilders) {
        builder(scriptEnv);
    }
}

#endif // !Vorb_ScriptedMod_h__
