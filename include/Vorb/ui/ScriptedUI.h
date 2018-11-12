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

DECL_VG(class SpriteBatch; class SpriteFont)

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
        // TODO(Matthew): Provide method to load textures.
        // TODO(Matthew): Make a non-scripted version.
        // TODO(Matthew): ValueMediator specialisations for Length & Length2.
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
            void init(const GameWindow* window, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
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
            Viewport* makeViewFromScript(const nString& name, ZIndex zIndex, const vio::File& filepath);

            /*!
             * \brief Creates a new view in this UI.
             *
             * \param name The name of the new view.
             * \param zIndex The z-index of the new view.
             * \param filepath The path to the script to begin building from.
             *
             * \return A pointer to the viewport of the constructed view.
             */
            Viewport* makeViewFromYAML(const nString& name, ZIndex zIndex, const vio::File& filepath);

            /*!
             * \brief Gets the named view.
             *
             * \param name The name of the view to get.
             *
             * \return A pointer to the viewport of the view found, or nullptr if no view found as named.
             */
            Viewport* getView(const nString& name);

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
            Viewport* setViewZIndex(const nString& name, ZIndex zIndex);

            /*!
             * \brief Runs a script 
             *
             * \param name The name of the view to get.
             *
             * \return A pointer to the viewport of the view found, or nullptr if no view found as named.
             */
            Viewport* runViewScript(const nString& name, const vio::File& filepath);

            // TODO(Matthew): Other run funcs? runViewScriptFunc? runViewScript capturing multiple views?
        protected:
            VORB_NON_COPYABLE(ScriptedUI);

            void prepareScriptEnv(ScriptEnv* scriptEnv);

            UIViews           m_views;       ///< List of UI views in draw order.
            const GameWindow* m_window;      ///< Pointer to the window the UI views will be drawn to.
            vg::SpriteFont*   m_defaultFont; ///< Default font of views.
            vg::SpriteBatch*  m_spriteBatch; ///< SpriteBatch instance to use for rendering.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ScriptedUI_cpp__
