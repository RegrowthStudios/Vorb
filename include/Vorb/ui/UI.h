//
// UI.cpp
// Vorb Engine
//
// Created by Matthew Marshall 1st Dec 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file UI.cpp
* @brief Holds a set of constructed views and provides interfaces to manipulate them.
*/

#pragma once

#ifndef Vorb_UI_cpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UI_cpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <map>

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/File.h"

DECL_VG(class TextureCache; class SpriteBatch; class SpriteFont)
DECL_VUI(class IGameScreen)

namespace vorb {
    namespace ui {

        // Forward declarations
        class GameWindow;
        class Viewport;
        struct Length2;

        class UI {
        public:
            using ZIndex  = ui16;
            using UIViews = std::map<ZIndex, Viewport*>;

            /*!
             * \brief Just ensures UI instance is in valid state, use init to prepare instance for use.
             */
            UI();
            /*!
             * \brief Does nothing, call dispose!
             */
            ~UI();

            /*!
             * \brief Initialises the instance for use.
             *
             * \param ownerScreen The screen that owns this scripted UI.
             * \param window The game window this UI will be rendered to.
             * \param textureCache The cache to use for textures used by this UI.
             * \param defaultFont The default font for views to use.
             * \param spriteBatch The spritebatch for view renderers to use.
             */
            virtual void init(vui::IGameScreen* ownerScreen, const GameWindow* window, vg::TextureCache* textureCache, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            /*!
             * \brief Disposes the managed views and resets pointers.
             */
            virtual void dispose();

            /*!
             * \brief Tells each view to update.
             *
             * Views called in order of z-index.
             *
             * \param dt The time delta between this update and the last.
             */
            virtual void update(f32 dt = 0.0f);
            /*!
             * \brief Tells each view to draw.
             *
             * Views called in order of z-index.
             */
            virtual void draw();

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
             * \param defaultFont The default font of the view (overrides the default set for the UI instance if not nullptr).
             * \param spriteBatch The spritebatch to use for rendering.
             *
             * \return Pointers to the viewport of the constructed view.
             */
            Viewport* makeView(const nString& name, ZIndex zIndex, const f32v4& dimensions = f32v4(0.0f), vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
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
            Viewport* makeView(const nString& name, ZIndex zIndex, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);

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
        protected:
            VORB_NON_COPYABLE(UI);

            vui::IGameScreen* m_ownerScreen;  ///< The screen that owns this UI.
            UIViews           m_views;        ///< List of UI views in draw order.
            const GameWindow* m_window;       ///< Pointer to the window the UI views will be drawn to.
            vg::SpriteFont*   m_defaultFont;  ///< Default font of views.
            vg::SpriteBatch*  m_spriteBatch;  ///< SpriteBatch instance to use for rendering.
            vg::TextureCache* m_textureCache; ///< Cache for UI-related textures.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UI_cpp__
