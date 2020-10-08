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
#include "Vorb/ui/UI.h"
#include "Vorb/ui/script/ViewScriptContext.hpp"

#include <unordered_map>

DECL_VG(class FontCache; class TextureCache; class SpriteBatch; class SpriteFont)
DECL_VIO(class IOManagerBase)
DECL_VUI(class GameWindow; class IGameScreen; class UIBase)

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

            /*!
             * \brief Register a previously constructed UI with this registry, assigning it the given name.
             *
             * \param name: The name to assign the UI.
             * \param ui: The UI to register.
             *
             * \return True if the UI was registered, false if a UI was already registered with the given name.
             */
            bool registerUI(const nString& name, CALLER_DELETE UIBase* ui);
            /*!
             * \brief Register a previously constructed UI with this registry, assigning it the given name.
             *
             * \param ui: The UI to register and the name to which it should be assigned.
             *
             * \return True if the UI was registered, false if a UI was already registered with the given name.
             */
            bool registerUI(std::pair<nString, CALLER_DELETE UIBase*>&& ui);

            /*!
             * \brief Create a new UI within this registry, assigning it the given name.
             *
             * \param name: The name to assign the UI.
             * \param ownerScreen: The screen to which the UI is attached.
             * \param window: The window in which the game is embedded.
             * \param ioManager: The IO manager for acquiring files such as YAML files and scripts.
             * \param textureCache: The cache used for obtaining textures.
             * \param fontCache: The font used for rendering text in the UI.
             * \param spriteBatch: The batcher used for rendering other sprites of the UI.
             * \param spriteFont: Optional default font to use for views created in the UI.
             *
             * \return True if the UI was registered, false if a UI was already registered with the given name.
             */
            virtual CALLER_DELETE UIBase* makeUI(
                     const nString& name,
                       IGameScreen* ownerScreen,
                  const GameWindow* window,
                vio::IOManagerBase* ioManager,
                  vg::TextureCache* textureCache,
                     vg::FontCache* fontCache,
                   vg::SpriteBatch* spriteBatch,
                    vg::SpriteFont* defaultFont = nullptr
            ) = 0;

            /*!
             * \brief Obtain a registered UI with the given name.
             *
             * \param name: The name of the UI to obtain.
             *
             * \return A pointer to the UI named if found, nullptr otherwise.
             */
            UIBase* getUI(const nString& name);

            /*! \return The list of registered UIs.
             */
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

            virtual CALLER_DELETE UIBase* makeUI(
                     const nString& name,
                       IGameScreen* ownerScreen,
                  const GameWindow* window,
                vio::IOManagerBase* ioManager,
                  vg::TextureCache* textureCache,
                     vg::FontCache* fontCache,
                   vg::SpriteBatch* spriteBatch,
                    vg::SpriteFont* defaultFont /*= nullptr*/
            ) override {
                UI<ScriptEnvironment>* ui = new UI<ScriptEnvironment>();
                ui->init(ownerScreen, window, ioManager, textureCache, fontCache, spriteBatch, defaultFont);

                if (!m_uis.emplace(std::make_pair(name, ui)).second) {
                    delete ui;
                    return nullptr;
                }

                return ui;
            }

            void prepareScriptEnv(ScriptEnvironment* scriptEnv) {
                UIScriptContext::injectInto(scriptEnv);

                ViewScriptContext::injectInto(scriptEnv);

                scriptEnv->setNamespace("UI");
                scriptEnv->addCDelegate("getUI", makeFunctor([&](nString name) {
                    return getUI(name);
                }));
                scriptEnv->setNamespaces();
            }
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

            virtual CALLER_DELETE UIBase* makeUI(
                     const nString& name,
                       IGameScreen* ownerScreen,
                  const GameWindow* window,
                vio::IOManagerBase* ioManager,
                  vg::TextureCache* textureCache,
                     vg::FontCache* fontCache,
                   vg::SpriteBatch* spriteBatch,
                    vg::SpriteFont* defaultFont /*= nullptr*/
            ) override {
                UI<>* ui = new UI<>();
                ui->init(ownerScreen, window, ioManager, textureCache, fontCache, spriteBatch, defaultFont);

                if (!m_uis.emplace(std::make_pair(name, ui)).second) {
                    delete ui;
                    return nullptr;
                }

                return ui;
            }
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UIRegistry_h__
