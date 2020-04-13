//
// Mod.h
// Vorb Engine
//
// Created by Matthew Marshall on 16 July 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file Mod.h
 * \brief Provides classes for mods installed or otherwise.
 * These provide such things as the mod's metadata, and
 * basic utilities such as an IO manager, texture cache, etc.
 */

#pragma once

#ifndef Vorb_Mod_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Mod_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/io/IOManager.h"
#include "Vorb/io/Keg.h"
#include "Vorb/io/Path.h"
#include "Vorb/mod/ModIOManager.h"

namespace vorb {
    namespace mod {
        struct ModMetadata {
            nString name;
            nString author;
            nString version;
            // TODO(Matthew): More metadata info.
        };
        const ModMetadata NULL_METADATA = {
            "", ""
        };

        struct ModEntryPoints {
            Array<nString> blockFiles;
            Array<nString> caFiles;
            Array<nString> musicFiles;
            // TODO(Matthew): Expand as features get implemented, e.g. ecs stuff, creatures, ore gen, etc.
        };

        const nString METADATA_FILENAME     = "mod.yaml";
        const nString ENTRY_POINTS_FILENAME = "entry_points.yaml";
        const nString MANIFEST_FILENAME     = "manifest.yaml";

        class ModBase {
        public:
            /*!
             * \brief Initialises the mod, setting up necessary data, including loading
             * associated metadata.
             *
             * \param installDir: The directory where the mod is or would be installed.
             * \param referenceDir: The directory where the initial mod download is stored.
             *
             * \return True if the mod was correctly initialised, false otherwise.
             */
            virtual bool init(const vio::Path& installDir, const vio::Path& referenceDir);
            /*!
             * \brief Disposes of the mod.
             */
            virtual void dispose();

            /*!
             * \brief Starts up the mod.
             */
            virtual bool startup() = 0;
            /*!
             * \brief Shuts down the mod.
             */
            virtual bool shutdown() = 0;

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f) = 0;

            /*!
             * \return Provide a reference to the mod metadata.
             */
            const ModMetadata& getModMetadata() const {
                return m_metadata;
            }
        protected:
            /*!
             * \brief Loads metadata.
             *
             * The metadata is specified in the file "mod.yaml".
             *
             * \return True if metadata loaded correctly, false otherwise.
             */
            bool loadMetadata();

            ModMetadata m_metadata; ///< Metadata of mod, such as name, author, etc.

            bool m_isInstalled; ///< Whether mod is installed.
            bool m_isStarted; ///< Whether the mod is started.

            ModIOManager   m_activeIOManager; ///< IO manager used by the mod itself.
            vio::IOManager m_installIOManager; ///< IO manager used to install/uninstall mod.
        };
        using ModBases = std::vector<ModBase>;

        template <typename ScriptEnvironment = void, typename Enable = void>
        class Mod;

        template <typename ScriptEnvironment>
        class Mod<ScriptEnvironment,
                    typename std::enable_if<!std::is_void<ScriptEnvironment>::value>::type>
            : public ModBase {
        public:
            Mod();
            ~Mod() {
                // Empty.
            }

            // TODO(Matthew): Determine start up and shut down processes for mods.
            //                    For scripted this will be a bit more obvious, but YAML this may still be TBD.
            /*!
             * \brief Starts up the mod.
             */
            virtual bool startup() override {
                return true;
            }
            /*!
             * \brief Shuts down the mod.
             */
            virtual bool shutdown() override {
                return true;
            }

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f) override {
                // Empty.
            }

            /*!
             * \brief Registers a script env builder to be used for building a mod's script environment.
             *
             * Note: These builders should be registered by subsystems in order for them to register constants, C callbacks
             * and so forth to the script environment.
             *
             * \param scriptEnvBuilder The script environment builder to be registered.
             */
            static void registerScriptEnvBuilder(Delegate<ScriptEnvironment*>&& scriptEnvBuilder) {
                scriptEnvBuilders.emplace_back(std::forward<Delegate<ScriptEnvironment*>>(scriptEnvBuilder));
            }
        protected:
            /*!
             * \brief Builds the script environment.
             *
             * \param scriptEnv The script environment to build.
             */
            static void buildScriptEnv(ScriptEnvironment* scriptEnv) {
                for (auto& builder : scriptEnvBuilders) {
                    builder(scriptEnv);
                }
            }

            static std::vector<Delegate<ScriptEnvironment*>> scriptEnvBuilders; ///< The list of builders to use in building a script environment.

            ScriptEnvironment m_scriptEnv;
        };

        template <typename ScriptEnvironment>
        class Mod<ScriptEnvironment,
                    typename std::enable_if<std::is_void<ScriptEnvironment>::value>::type>
            : public ModBase {
        public:
            Mod();
            ~Mod() {
                // Empty.
            }

            // TODO(Matthew): Determine start up and shut down processes for mods.
            //                    For scripted this will be a bit more obvious, but YAML this may still be TBD.
            /*!
             * \brief Starts up the mod.
             *
             * \return True if successfully started, false otherwise.
             */
            virtual bool startup() {
                return true;
            }
            /*!
             * \brief Shuts down the mod.
             *
             * \return True if successfully shutdown, false otherwise.
             */
            virtual bool shutdown() {
                return true;
            }

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt: The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f) {
                // Empty.
            }
        };

        template <typename ScriptEnvironment = void>
        using Mods = std::vector<Mod<ScriptEnvironment>>;
    }
}
namespace vmod = vorb::mod;

KEG_TYPE_DECL(ModMetadata);
KEG_TYPE_DECL(ModEntryPoints);

#endif // !Vorb_Mod_h__
