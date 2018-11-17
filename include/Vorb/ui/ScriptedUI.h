//
// ScriptedUI.cpp
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ScriptedUI.cpp
* @brief
* High level UI class that is defined
* with scripting.
*/

#pragma once

#ifndef Vorb_ScriptedUI_cpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ScriptedUI_cpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <map>

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/File.h"
// TODO(Matthew): Enable adding more directories to look in for textures (e.g. so each mod can have its own texture dir / to support layering texture packs).
#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/script/ViewScriptEnvironment.h"

DECL_VG(class SpriteBatch; class SpriteFont; class TextureCache)

namespace vorb {
    namespace ui {

        // Forward declarations
        class GameWindow;
        class Viewport;
        template <typename ScriptEnvironmentImpl>
        class ViewScriptEnvironment;
        struct Length2;

        // Pairing of Viewport and script environment, forming a view.
        template <typename ScriptEnvironmentImpl>
        struct ScriptedView {
            Viewport* viewport;
            ViewScriptEnvironment<ScriptEnvironmentImpl>* scriptEnv;
        };

        // TODO(Matthew): Subscription to widget events.
        // TODO(Matthew): Unsubscription from events (also in vscript::IEnvironment!).
        // TODO(Matthew): Make a non-scripted version.
        template <typename ScriptEnvironmentImpl>
        class ScriptedUI {
        public:
            using ZIndex    = ui16;
            using ScriptEnv = ViewScriptEnvironment<ScriptEnvironmentImpl>;
            using UIViews   = std::map<ZIndex, ScriptedView<ScriptEnvironmentImpl>>;

            /*!
             * \brief Just ensures ScriptedUI instance is in valid state, use init to prepare instance for use.
             */
            ScriptedUI();
            /*!
             * \brief Does nothing, call dispose!
             */
            ~ScriptedUI();

            /*!
             * \brief Initialises the instance for use.
             *
             * \param window The game window this UI will be rendered to.
             * \param defaultFont The default font for views to use.
             * \param spriteBatch The spritebatch for view renderers to use.
             */
            void init(const GameWindow* window, vg::TextureCache* textureCache, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            /*!
             * \brief Disposes the managed views and resets pointers.
             */
            void dispose();

            /*!
             * \brief Tells each view to update.
             *
             * Views called in order of z-index.
             *
             * \param dt The time delta between this update and the last.
             */
            void update(f32 dt = 0.0f);
            /*!
             * \brief Tells each view to draw.
             *
             * Views called in order of z-index.
             */
            void draw();

            /*!
             * \brief Gets the texture cache used by the UI.
             *
             * \return A pointer to the texture cache used.
             */
            vg::TextureCache* getTextureCache() { return m_textureCache; }

            /*!
             * \brief Sets the texture cache used by the UI.
             *
             * \param Pointer to the texture cache to be set.
             */
            void setTextureCache(vg::TextureCache* textureCache) { m_textureCache = textureCache; }

            // TODO(Matthew): Hook this into some options state event? This would involve Vorb providing a game options supporting class - and optons might not be a well-generalisable thing. Worst case can put the onus on Vorb users to register it to an event/call it directly.
            /*!
             * \brief Handles options changing.
             */
            void onOptionsChanged();

            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param dimensions The dimensions of the viewport.
             * \param defaultFont The default font of the view (overrides the default set for the ScriptedUI instance if not nullptr).
             * \param spriteBatch The spritebatch to use for rendering.
             *
             * \return Pointers to the viewport and script env of the constructed view.
             */
            ScriptedView<ScriptEnvironmentImpl> makeView(const nString& name, ZIndex zIndex, const f32v4& dimensions = f32v4(0.0f), vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param position The position of the viewport.
             * \param size The size of the viewport.
             * \param defaultFont The default font of the view (overrides the default set for the ScriptedUI instance if not nullptr).
             * \param spriteBatch The spritebatch to use for rendering.
             *
             * \return Pointers to the viewport and script env of the constructed view.
             */
            ScriptedView<ScriptEnvironmentImpl> makeView(const nString& name, ZIndex zIndex, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);

            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param filepath The path to the script to begin building from.
             *
             * \return A pointer to the viewport of the constructed view.
             */
            Viewport* makeViewFromScript(nString name, ZIndex zIndex, nString filepath);

            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param filepath The path to the script to begin building from.
             *
             * \return A pointer to the viewport of the constructed view.
             */
            Viewport* makeViewFromYAML(nString name, ZIndex zIndex, nString filepath);

            /*!
             * \brief Gets the named view.
             *
             * \param name The name of the view to get.
             *
             * \return A pointer to the viewport of the view found, or nullptr if no view found as named.
             */
            Viewport* getView(nString name);

            /*!
             * \brief Gets the named view's script environment.
             *
             * \param name The name of the view to get.
             *
             * \return A pointer to the view script env of the view found, or nullptr if no view found as named.
             */
            ScriptEnv* getEnv(const nString& name);

            /*!
             * \brief Enables the pointed-to view.
             *
             * \param viewport Pointer to the viewport to enable.
             *
             * \return The pointer provided.
             */
            Viewport* enableView(Viewport* viewport);
            /*!
             * \brief Enables the named view.
             *
             * \param name The name of the view to enable.
             *
             * \return A pointer to the viewport of the view enabled, or nullptr if no view found as named.
             */
            Viewport* enableViewWithName(nString name);
            /*!
             * \brief Disables the pointed-to view.
             *
             * \param viewport Pointer to the viewport to disable.
             *
             * \return The pointer provided.
             */
            Viewport* disableView(Viewport* viewport);
            /*!
             * \brief Disables the named view.
             *
             * \param name The name of the view to disable.
             *
             * \return A pointer to the viewport of the view disabled, or nullptr if no view found as named.
             */
            Viewport* disableViewWithName(nString name);


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
            bool destroyViewWithName(nString name);

            /*!
             * \brief Sets a new z-index value for the named view.
             *
             * \param name The name of the view to change the z-index of.
             *
             * \return A pointer to the viewport of the named view, or nullptr if no view found as named.
             */
            Viewport* setViewZIndex(nString name, ZIndex zIndex);

            /*!
             * \brief Runs a script 
             *
             * \param name The name of the view to get.
             *
             * \return A pointer to the viewport of the view found, or nullptr if no view found as named.
             */
            Viewport* runViewScript(const nString& name, const vio::Path& filepath);

            // TODO(Matthew): Other run funcs? runViewScriptFunc? runViewScript capturing multiple views?
        protected:
            VORB_NON_COPYABLE(ScriptedUI);

            void prepareScriptEnv(ScriptEnv* scriptEnv);

            UIViews           m_views;        ///< List of UI views in draw order.
            const GameWindow* m_window;       ///< Pointer to the window the UI views will be drawn to.
            vg::SpriteFont*   m_defaultFont;  ///< Default font of views.
            vg::SpriteBatch*  m_spriteBatch;  ///< SpriteBatch instance to use for rendering.
            vg::TextureCache* m_textureCache; ///< Cache for UI-related textures.
        };
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
vui::ScriptedUI<ScriptEnvironmentImpl>::ScriptedUI() :
    m_window(nullptr),
    m_defaultFont(nullptr),
    m_spriteBatch(nullptr) {
    // Empty
}

template <typename ScriptEnvironmentImpl>
vui::ScriptedUI<ScriptEnvironmentImpl>::~ScriptedUI() {
    // Empty
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::init(const GameWindow* window, vg::TextureCache* textureCache, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_window       = window;
    m_textureCache = textureCache;
    m_defaultFont  = defaultFont;
    m_spriteBatch  = spriteBatch;
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::dispose() {
    for (auto& view : m_views) {
        view.second.viewport->dispose();
        view.second.scriptEnv->dispose();
        delete view.second.viewport;
        delete view.second.scriptEnv;
    }
    UIViews().swap(m_views);

    m_window      = nullptr;
    m_defaultFont = nullptr;
    m_spriteBatch = nullptr;
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::update(f32 dt /*= 1.0f*/) {
    for (auto& view : m_views) {
        // TODO(Matthew): Implement script env updating.
        // view.second.scriptEnv->update(dt);
        view.second.viewport->update(dt);
    }
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::draw() {
    for (auto& view : m_views) {
        view.second.viewport->draw();
    }
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::onOptionsChanged() {
    // TODO(Matthew): Implement.
}

template <typename ScriptEnvironmentImpl>
vui::ScriptedView<ScriptEnvironmentImpl> vui::ScriptedUI<ScriptEnvironmentImpl>::makeView(const nString& name, ZIndex zIndex, const f32v4& dimensions /*= f32v4(0.0f)*/, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    ScriptedView<ScriptEnvironmentImpl> view;

    view.viewport = new Viewport(m_window);
    view.viewport->init(name, dimensions, defaultFont ? defaultFont : m_defaultFont, spriteBatch ? spriteBatch : m_spriteBatch);

    view.scriptEnv = new ScriptEnv();
    view.scriptEnv->init(view.viewport, m_window);

    prepareScriptEnv(view.scriptEnv);

    m_views.insert(std::make_pair(zIndex, view));

    return view;
}

template <typename ScriptEnvironmentImpl>
vui::ScriptedView<ScriptEnvironmentImpl> vui::ScriptedUI<ScriptEnvironmentImpl>::makeView(const nString& name, ZIndex zIndex, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    ScriptedView<ScriptEnvironmentImpl> view;

    view.viewport = new Viewport(m_window);
    view.viewport->init(name, position, size, defaultFont ? defaultFont : m_defaultFont, spriteBatch ? spriteBatch : m_spriteBatch);

    view.scriptEnv = new ScriptEnv();
    view.scriptEnv->init(view.viewport, m_window);

    prepareScriptEnv(view.scriptEnv);

    m_views.insert(std::make_pair(zIndex, view));

    return view;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::makeViewFromScript(nString name, ZIndex zIndex, nString filepath) {
    ScriptedView<ScriptEnvironmentImpl> view = makeView(name, zIndex);

    view.scriptEnv->run(vio::Path(filepath));

    return view.viewport;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::makeViewFromYAML(nString name, ZIndex zIndex, nString filepath VORB_UNUSED) {
    ScriptedView<ScriptEnvironmentImpl> view = makeView(name, zIndex);

    // TODO(Matthew): Implement building view from YAML.

    return view.viewport;
}

// TODO(Matthew): Do we want to sort by name, either not guaranteeing order of render of each viewport, or storing the information twice?
template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::getView(nString name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            return view.second.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::ViewScriptEnvironment<ScriptEnvironmentImpl>* vui::ScriptedUI<ScriptEnvironmentImpl>::getEnv(const nString& name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            return view.second.scriptEnv;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::enableView(Viewport* viewport) {
    viewport->enable();

    return viewport;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::enableViewWithName(nString name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            view.second.viewport->enable();
            return view.second.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::disableView(Viewport* viewport) {
    viewport->disable();

    return viewport;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::disableViewWithName(nString name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            view.second.viewport->disable();
            return view.second.viewport;
        }
    }
    return nullptr;
}


template <typename ScriptEnvironmentImpl>
bool vui::ScriptedUI<ScriptEnvironmentImpl>::destroyView(Viewport* viewport) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second.viewport == viewport) {
            // Dispose the viewport and contained widgets and the script environment of the viewport.
            view.second.viewport->dispose();
            view.second.scriptEnv->dispose();

            // Free our memory.
            delete view.second.viewport;
            delete view.second.scriptEnv;

            // Erase view from the list of views.
            m_views.erase(it);

            return true;
        }
    }
    return false;
}

template <typename ScriptEnvironmentImpl>
bool vui::ScriptedUI<ScriptEnvironmentImpl>::destroyViewWithName(nString name) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second.viewport->getName() == name) {
            // Dispose the viewport and contained widgets and the script environment of the viewport.
            view.second.viewport->dispose();
            view.second.scriptEnv->dispose();

            // Free our memory.
            delete view.second.viewport;
            delete view.second.scriptEnv;

            // Erase view from the list of views.
            m_views.erase(it);

            return true;
        }
    }
    return false;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::setViewZIndex(nString name, ZIndex zIndex) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second.viewport->getName() == name) {
            ScriptedView<ScriptEnvironmentImpl> newView;
            newView.viewport  = view.second.viewport;
            newView.scriptEnv = view.second.scriptEnv;

            m_views.erase(it);

            m_views.insert(std::make_pair(zIndex, newView));

            return newView.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::runViewScript(const nString& name, const vio::Path& filepath) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            view.second.scriptEnv->run(filepath);
            return view.second.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::prepareScriptEnv(ScriptEnv* scriptEnv) {
    vscript::IEnvironment<ScriptEnvironmentImpl>* env = scriptEnv->getEnv();

    env->setNamespaces("UI");
    env->addCDelegate("makeViewFromScript",  makeDelegate(this, &ScriptedUI::makeViewFromScript));
    env->addCDelegate("makeViewFromYAML",    makeDelegate(this, &ScriptedUI::makeViewFromYAML));
    env->addCDelegate("enableView",          makeDelegate(this, &ScriptedUI::enableView));
    env->addCDelegate("enableViewWithName",  makeDelegate(this, &ScriptedUI::enableViewWithName));
    env->addCDelegate("disableView",         makeDelegate(this, &ScriptedUI::disableView));
    env->addCDelegate("disableViewWithName", makeDelegate(this, &ScriptedUI::disableViewWithName));
    env->addCDelegate("getView",             makeDelegate(this, &ScriptedUI::getView));
    env->addCDelegate("setViewZIndex",       makeDelegate(this, &ScriptedUI::setViewZIndex));
    env->addCDelegate("destroyView",         makeDelegate(this, &ScriptedUI::destroyView));
    env->addCDelegate("destroyViewWithName", makeDelegate(this, &ScriptedUI::destroyViewWithName));
    env->setNamespaces();
}

#endif // !Vorb_ScriptedUI_cpp__
