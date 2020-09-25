//
// Mod.h
// Vorb Engine
//
// Created by Matthew Marshall on 16 July 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file Mod.h
 * \brief Provides classes for mods that handle startup and
 * shutdown procedures, as well as setting up a script
 * environment where needed.
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
            "", "", ""
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
            ModBase();
            virtual ~ModBase() {
                // Empty.
            }

            /*!
             * \brief Initialises the mod, setting up necessary data.
             *
             * \param metadata: The metadata of the mod.
             * \param modDir: The directory of the mod.
             * \param dataAssetIOManager: The data asset IO manager to be used for read operations.
             *
             * \return True if the mod was correctly initialised, false otherwise.
             */
            virtual void init(ModMetadata metadata, const vio::Path& modDir, const DataAssetIOManager* dataAssetIOManager);
            /*!
             * \brief Disposes of the mod.
             */
            virtual void dispose();

            /*!
             * \brief Starts up the mod.
             */
            virtual bool startup() const = 0;
            /*!
             * \brief Shuts down the mod.
             */
            virtual bool shutdown() const = 0;

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f) const = 0;

            /*! \return A reference to the mod metadata.
             */
            const ModMetadata& getModMetadata() const {
                return m_metadata;
            }
            /*! \return The directory of the mod.
             */
            const vio::Path& getModDir() const {
                return m_modDir;
            }
        protected:
            ModMetadata m_metadata; ///< Metadata of mod, such as name, author, etc.
            vio::Path m_modDir;

            bool m_isStarted; ///< Whether the mod is started.

            ModIOManager m_ioManager; ///< IO manager used by the mod itself.
        };
        using ModBases = std::vector<ModBase>;
        using ModBasePtrs = std::vector<const ModBase*>;

        template <typename ScriptEnvironment = void, typename Enable = void>
        class Mod;

        template <typename ScriptEnvironment>
        class Mod<ScriptEnvironment,
                    typename std::enable_if<!std::is_void<ScriptEnvironment>::value>::type>
            : public ModBase {
        public:
            Mod() {
                // Empty.
            }
            ~Mod() {
                // Empty.
            }

            /*!
             * \brief Initialises the mod, setting up necessary data.
             *
             * \param metadata: The metadata of the mod.
             * \param modDir: The directory of the mod.
             * \param dataAssetIOManager: The data asset IO manager to be used for read operations.
             * \param scriptEnv: The script environment used by the mod.
             *
             * \return True if the mod was correctly initialised, false otherwise.
             */
            void init(
                ModMetadata metadata,
                const vio::Path& modDir,
                const DataAssetIOManager* dataAssetIOManager,
                CALLEE_DELETE ScriptEnvironment* scriptEnv
            ) {
                ModBase::init(metadata, modDir, dataAssetIOManager);

                m_scriptEnv = scriptEnv;
            }
            /*!
             * \brief Disposes of the mod.
             */
            virtual void dispose() override {
                m_scriptEnv->dispose();
                delete m_scriptEnv;
                m_scriptEnv = nullptr;

                ModBase::dispose();
            }

            // TODO(Matthew): Determine start up and shut down processes for mods.
            //                    For scripted this will be a bit more obvious, but YAML this may still be TBD.
            /*!
             * \brief Starts up the mod.
             */
            virtual bool startup() const override {
                return true;
            }
            /*!
             * \brief Shuts down the mod.
             */
            virtual bool shutdown() const override {
                return true;
            }

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f) const override {
                // Empty.
            }
        protected:
            ScriptEnvironment* m_scriptEnv;
        };

        template <typename ScriptEnvironment>
        class Mod<ScriptEnvironment,
                    typename std::enable_if<std::is_void<ScriptEnvironment>::value>::type>
            : public ModBase {
        public:
            Mod() {
                // Empty.
            }
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
            virtual bool startup() const override {
                return true;
            }
            /*!
             * \brief Shuts down the mod.
             *
             * \return True if successfully shutdown, false otherwise.
             */
            virtual bool shutdown() const override {
                return true;
            }

            // TODO(Matthew): Once we implement mod timing, we may find a mod is forced to skip frames. We will need to accumulate dt in that case, maybe do other things.
            /*!
             * \brief Run update loop of the mod.
             *
             * \param dt: The time since the last frame.
             */
            virtual void update(f32 dt = 0.0f) const override {
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
