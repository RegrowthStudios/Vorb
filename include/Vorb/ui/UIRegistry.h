//
// UIRegistry.h
// Vorb Engine
//
// Created by Matthew Marshall on 27 Sept 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file UIRegistry.h
 * \brief Registry for Vorb UIs. A set of events are exposed through this to any associated script environment for Lua scripts
 * to interact with all UIs, created by them or otherwise.
 */

#pragma once

#ifndef Vorb_UIRegistry_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UIRegistry_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"

#include <unordered_map>

DECL_VUI(class UIBase)

namespace vorb {
    namespace ui {
        using RegisteredUIs = std::unordered_map<nString, UIBase*>;

        class UIRegistryBase {
        public:
            UIRegistryBase() {
                // Empty.
            }
            virtual ~UIRegistryBase() {
                // Empty.
            }

            void init();
            void dispose();

            // TODO(Matthew): Implement a UIBase class for UI & ScriptedUI.
            bool registerUI(const nString& name, UIBase* ui);
            bool registerUI(std::pair<nString, UIBase*>&& ui);

            UIBase* getUI(const nString& name);

            const RegisteredUIs& getUIs() { return m_uis; }

        protected:
            RegisteredUIs m_uis;
        };

        template <typename ScriptEnvironment = void, typename Enable = void>
        class UIRegistry;

        template <typename ScriptEnvironment>
        class UIRegistry<ScriptEnvironment,
                    typename std::enable_if<!std::is_void<ScriptEnvironment>::value>::type>
            : public UIRegistryBase {
        public:
            UIRegistry() {
                // Empty.
            }
            virtual ~UIRegistry() {
                // Empty.
            }

            void prepareScriptEnv(ScriptEnvironment* scriptEnv);
        };

        template <typename ScriptEnvironment>
        class UIRegistry<ScriptEnvironment,
                    typename std::enable_if<std::is_void<ScriptEnvironment>::value>::type>
            : public UIRegistryBase {
        public:
            UIRegistry() {
                // Empty.
            }
            ~UIRegistry() {
                // Empty.
            }
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UIRegistry_h__
