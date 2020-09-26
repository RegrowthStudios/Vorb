//
// UILoader.h
// Vorb Engine
//
// Created by Matthew Marshall 1 Dec 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file UILoader.h
* @brief Loads a UI from YAML.
*/

#pragma once

#ifndef Vorb_UILoader_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UILoader_h__
//! @endcond

#include "Vorb/Delegate.hpp"
#include "Vorb/types.h"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/YAMLNode.h"

DECL_VG(class TextureCache)
DECL_VIO(class IOManager)
DECL_VUI(class IWidget; class Viewport)

namespace vorb {
    namespace ui {
        // TODO(Matthew): Would be nice to have a way to cache UI, but it'll be bloody hard because of the event hooks and such
        //                that are not simple data. We can come back to this.
        namespace UILoader {
            using WidgetParser = Delegate<IWidget*, const nString&, keg::Node>;

            // TODO(Matthew): Implement this function.
            /*!
             * \brief Loads a widget and its descendents from a YAML file.
             *
             * \param vio::IOManager*   iom The IOManager instance to use for loading the YAML data.
             * \param const cString     filepath The filepath of the YAML data (relative to the IOManager instance's base).
             * \param vg::TextureCache* textureCache The texture cache to use for loading any of the widgets' textures from.
             * \param vui::UILoader::WidgetParser customWidgetParser A custom widget parser that may be optionally passed in to override default widget parsing.
             *
             * \return A pointer to the created widget (tree), or nullptr if creation failed.
             */
            IWidget* loadWidgetFromYAML(vio::IOManager* iom, const cString filepath, vg::TextureCache* textureCache, WidgetParser* customWidgetParser = nullptr);

            /*!
             * \brief Loads a widget and its descendents from a YAML file.
             *
             * \param vio::IOManager*   iom The IOManager instance to use for loading the YAML data.
             * \param const cString     filepath The filepath of the YAML data (relative to the IOManager instance's base).
             * \param vg::TextureCache* textureCache The texture cache to use for loading any of the widgets' textures from.
             * \param vui::Viewport*    viewport A pointer to the viewport to be used as a base for the loaded UI.
             * \param vui::UILoader::WidgetParser customWidgetParser A custom widget parser that may be optionally passed in to override default widget parsing. 
             */
            bool loadFromYAML(vio::IOManager* iom, const cString filepath, vg::TextureCache* textureCache, Viewport* viewport, WidgetParser* customWidgetParser = nullptr);
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UILoader_h__