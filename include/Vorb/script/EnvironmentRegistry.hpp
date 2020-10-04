//
// EnvironmentRegistry.h
// Vorb Engine
//
// Created by Matthew Marshall on 30 Sept 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file EnvironmentRegistry.h
 * \brief Provides means of obtaining script environments with shared global spaces.
 * 
 * Script environments can be obtained from this registry, with each unique global space being managed by the registry.
 *
 * A present limitation is that, with each group sharing their global space, each group must executed on a single thread.
 */

#pragma once

#ifndef Vorb_EnvironmentRegistry_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_EnvironmentRegistry_hpp__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include <unordered_map>

namespace vorb {
    namespace script {
        template <typename ScriptEnvironment>
        using ScriptEnvironmentGroup = std::pair<ScriptEnvironment*, std::vector<ScriptEnvironment*>>;
        template <typename ScriptEnvironment>
        using ScriptEnvironmentGroups = std::unordered_map<nString, ScriptEnvironmentGroup<ScriptEnvironment>>;

        template <typename ScriptEnvironment>
        using ScriptEnvironmentBuilder = Delegate<void, ScriptEnvironment*>;

        template <typename ScriptEnvironment>
        class EnvironmentRegistry {
        public:
            /*! \brief Initialises the registry.
             */
            virtual void init();
            /*!
             * \brief Disposes the registry, cleaning up the various script environments
             * present in the groups stored in the registry.
             */
            virtual void dispose();

            /*!
             * \brief Creates a new group for getting script environments from.
             *
             * Note that script environments in the same group share global data, but have
             * unique execution space.
             *
             * \param groupName: The name of the group to create.
             * \param builder: A callback that is used to build the context of
             * the script environment (i.e. exposed C functions & constants).
             *
             * \return True if the group was created, false otherwise.
             */
            virtual bool createGroup(const nString& groupName, ScriptEnvironmentBuilder<ScriptEnvironment>* builder = nullptr);

            /*!
             * \brief Gets a script environment within the given group. Fails if
             * the group named does not exist.
             *
             * Note that script environments in the same group share global data, but have
             * unique execution space.
             *
             * \param groupName: The name of the group to get a script environment from.
             *
             * \return Pointer to created script evnironment if group exists, nullptr otherwise.
             */
            virtual CALLEE_DELETE ScriptEnvironment* getScriptEnv(const nString& groupName);

        protected:
            ScriptEnvironmentGroups<ScriptEnvironment> m_scriptEnvGroups;
        };
    }
}
namespace vscript = vorb::script;

template <typename ScriptEnvironment>
void vscript::EnvironmentRegistry<ScriptEnvironment>::init() {
    // Empty.
}

template <typename ScriptEnvironment>
void vscript::EnvironmentRegistry<ScriptEnvironment>::dispose() {
    for (auto& group : m_scriptEnvGroups) {
        for (auto& spawnedEnv : group.second.second) {
            spawnedEnv->dispose();
            delete spawnedEnv;
        }

        group.second.first->dispose();
        delete group.second.first;
    }
    ScriptEnvironmentGroups<ScriptEnvironment>().swap(m_scriptEnvGroups);
}

template <typename ScriptEnvironment>
bool vscript::EnvironmentRegistry<ScriptEnvironment>::createGroup(const nString& groupName, ScriptEnvironmentBuilder<ScriptEnvironment>* builder /*= nullptr*/) {
    ScriptEnvironment* env = new ScriptEnvironment();
    env->init();

    if (builder != nullptr) builder->invoke(env);

    return m_scriptEnvGroups.insert(std::make_pair(groupName, std::make_pair(env, std::vector<ScriptEnvironment*>()))).second;
}

template <typename ScriptEnvironment>
CALLEE_DELETE ScriptEnvironment* vscript::EnvironmentRegistry<ScriptEnvironment>::getScriptEnv(const nString& groupName) {
    ScriptEnvironmentGroup<ScriptEnvironment> group;
    try {
        group = m_scriptEnvGroups.at(groupName);
    } catch (std::out_of_range& e) {
        return nullptr;
    }

    ScriptEnvironment* spawned = new ScriptEnvironment();
    spawned->init(group.first);

    group.second.push_back(spawned);

    return spawned;
}

#endif // VORB_USING_SCRIPT

#endif // !Vorb_EnvironmentRegistry_hpp__
