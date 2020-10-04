//
// UI.h
// Vorb Engine
//
// Created by Matthew Marshall 1st Dec 2018
// Rearchitected 27 Sept 2020
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file UI.h
* @brief Holds a set of constructed views and provides interfaces to manipulate them.
*/

#pragma once

#ifndef Vorb_UI_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UI_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <map>

#include "Vorb/Event.hpp"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Path.h"
#include "Vorb/ui/script/ViewScriptContext.hpp"
#include "Vorb/ui/widgets/Viewport.h"

DECL_VG(class FontCache; class TextureCache; class SpriteBatch)
DECL_VIO(class IOManagerBase)
DECL_VUI(class GameWindow; class IGameScreen; class Viewport)

namespace vorb {
    namespace ui {
        using ZIndex = ui16;
        using UIViews = std::map<ZIndex, Viewport*>;

        class UIBase {
        public:
            UIBase();
            virtual ~UIBase() {
                // Empty.
            }

            /*!
             * \brief Initialises the UI with the necessary services to acquire files, textures, render fonts and othe spritesm etc.
             *
             * \param ownerScreen: The screen to which this UI is attached.
             * \param window: The window in which the game is embedded.
             * \param ioManager: The IO manager for acquiring files such as YAML files and scripts.
             * \param textureCache: The cache used for obtaining textures.
             * \param spriteFont: The font used for rendering text in the UI.
             * \param spriteBatch: The batcher used for rendering other sprites of the UI.
             */
            virtual void init(
                       IGameScreen* ownerScreen,
                  const GameWindow* window,
                vio::IOManagerBase* ioManager,
                  vg::TextureCache* textureCache,
                     vg::FontCache* fontCache,
                   vg::SpriteBatch* spriteBatch
            );
            /*! \brief Disposes of the UI, removing all associated services.
             */
            virtual void dispose();

            /*!
             * \brief Performs an update of the UI, updating each view stored within.
             *
             * \param dt: The time elapsed since the last update.
             */
            virtual void update(f32 dt = 0.0f);
            /*! \brief Draws the UI to the screen.
             */
            virtual void draw();

                   IGameScreen* getOwnerScreen()  { return m_ownerScreen;  }
              const GameWindow* getGameWindow()   { return m_gameWindow;   }
            vio::IOManagerBase* getIOManager()    { return m_ioManager;    }
              vg::TextureCache* getTextureCache() { return m_textureCache; }
                 vg::FontCache* getFontCache()    { return m_fontCache;    }
               vg::SpriteBatch* getSpriteBatch()  { return m_spriteBatch;  }

            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param dimensions The dimensions of the viewport.
             *
             * \return Pointers to the viewport of the constructed view.
             */
            Viewport* makeView(const nString& name, ZIndex zIndex, const f32v4& dimensions = f32v4(0.0f));
            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param position The position of the viewport.
             * \param size The size of the viewport.
             * \param defaultFont The default font of the view (overrides the default set for the UI instance if not nullptr).
             * \param spriteBatch The spritebatch to use for rendering.
             *
             * \return Pointers to the viewport of the constructed view.
             */
            Viewport* makeView(const nString& name, ZIndex zIndex, const Length2& position, const Length2& size);

            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param filepath The path to the YAML file to begin building from.
             *
             * \return A pointer to the viewport of the constructed view.
             */
            Viewport* makeViewFromYAML(const nString& name, ZIndex zIndex, const nString& filepath);

            /*!
             * \brief Gets the named view.
             *
             * \param name The name of the view to get.
             *
             * \return A pointer to the viewport of the view found, or nullptr if no view found as named.
             */
            Viewport* getView(const nString& name);


            /*!
             * \brief Destroys the pointed-to view.
             *
             * \param viewport Pointer to the viewport to destroy.
             *
             * \return True if the view was destroyed, false otherwise.
             */
            bool destroyView(Viewport* viewport);
            /*!
             * \brief Destroys the named view.
             *
             * \param name The name of the view to destroy.
             *
             * \return True if the view was destroyed, false otherwise.
             */
            bool destroyViewWithName(const nString& name);

            /*!
             * \brief Sets a new z-index value for the pointed-to view.
             *
             * \param viewport True if view's z-index was updated, false otherwise.
             */
            bool setViewZIndex(Viewport* viewport, ZIndex zIndex);
            /*!
             * \brief Sets a new z-index value for the named view.
             *
             * \param name The name of the view to change the z-index of.
             *
             * \return True if named view's' z-index updated, false otherwise.
             */
            bool setViewWithNameZIndex(const nString& name, ZIndex zIndex);

            Event<f32> PreUpdate;
            Event<f32> PostUpdate;
            Event<>    PreDraw;
            Event<>    PostDraw;
        protected:
            VORB_NON_COPYABLE(UIBase);

            IGameScreen* m_ownerScreen; ///< The owning screen of this UI.
            const GameWindow* m_gameWindow; ///< The game window to which this UI is drawn.

            vio::IOManagerBase* m_ioManager; ///< The IO manager used to obtain needed assets.

            vg::TextureCache* m_textureCache; ///< The cache used to obtain textures used by this UI.
            // TODO(Matthew): When fetching from cache, render font at next power of 2 font size compared to size chosen. Do we need a limit?
            vg::FontCache* m_fontCache; ///< The cache used to obtain fonts used by this UI.
            vg::SpriteBatch* m_spriteBatch; ///< The batcher used to render elements of this UI.

            UIViews m_views; ///< The list of views that comprise this UI.
        };

        template <typename ScriptEnvironment = void, typename Enable = void>
        class UI;

        template <typename ScriptEnvironment>
        class UI<ScriptEnvironment,
                    typename std::enable_if<std::is_void<ScriptEnvironment>::value>::type>
            : public UIBase {
            // TODO(Matthew): Anything here? Maybe it's all in UIBase...
        };

        template <typename ScriptEnvironment>
        class UI<ScriptEnvironment,
                    typename std::enable_if<!std::is_void<ScriptEnvironment>::value>::type>
            : public UIBase {
        public:
            /*!
             * \brief Creates a new view in this UI from the named script.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param filepath The path to the script to begin building from.
             *
             * \return A pointer to the viewport of the constructed view.
             */
            Viewport* makeViewFromScript(const nString& name, ZIndex zIndex, const nString& filepath) {
                Viewport* view = makeView(name, zIndex);

                ScriptEnvironment scriptEnv;
                scriptEnv.init();

                ViewScriptContext::injectInto(scriptEnv);

                scriptEnv->setNamespaces("UI", "View");
                scriptEnv->addValue("port", view);
                scriptEnv->setNamespaces();

                scriptEnv->run(vio::Path(filepath));

                return view;
            }
        };

        // TODO(Matthew): Handle situations where we want to provide further injections for certain UIs.
        //                    E.g. as in MainMenuScriptedUI.

        namespace UIScriptContext {
            template <typename ScriptEnvironment>
            void injectUIScriptContext(ScriptEnvironment* env) {
                env->setNamespaces("UI");
                env->addCDelegate("makeView", makeFunctor([](UIBase* ui, nString name, ZIndex zIndex) {
                    return ui->makeView(name, zIndex);
                }));
                env->addCDelegate("makeViewFromYAML", makeFunctor([](UIBase* ui, nString name, ZIndex zIndex, nString filepath) {
                    return ui->makeViewFromYAML(name, zIndex, filepath);
                }));
                env->addCDelegate("getView", makeFunctor([](UIBase* ui, nString name) {
                    return ui->getView(name);
                }));
                env->addCDelegate("destroyView", makeFunctor([](UIBase* ui, Viewport* view) {
                    return ui->destroyView(view);
                }));
                env->addCDelegate("destroyViewWithName", makeFunctor([](UIBase* ui, nString name) {
                    return ui->destroyViewWithName(name);
                }));
                env->addCDelegate("setViewZIndex", makeFunctor([](UIBase* ui, Viewport* view, ZIndex zIndex) {
                    return ui->setViewZIndex(view, zIndex);
                }));
                env->addCDelegate("setViewZIndexWithName", makeFunctor([](UIBase* ui, nString name, ZIndex zIndex) {
                    return ui->setViewZIndexWithName(name, zIndex);
                }));
                env->setNamespaces();
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UI_h__
