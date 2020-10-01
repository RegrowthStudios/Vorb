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
DECL_VIO(class IOManagerBase)
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
             * \param ioManager The IOManagerBase instance to use for loading the YAML data.
             * \param filepath The filepath of the YAML data to use.
             * \param textureCache The texture cache to use for loading any of the widgets' textures from.
             * \param customWidgetParser A custom widget parser that may be optionally passed in to override default widget parsing.
             *
             * \return A pointer to the created widget (tree), or nullptr if creation failed.
             */
            IWidget* loadWidgetFromYAML(vio::IOManagerBase* ioManager, const cString filepath, vg::TextureCache* textureCache, WidgetParser* customWidgetParser = nullptr);

            /*!
             * \brief Loads a view from a YAML file.
             *
             * \param viewport The viewport to which the view built is attached.
             * \param ioManager The IOManagerBase instance to use for loading the YAML data.
             * \param filepath The filepath of the YAML data to use.
             * \param textureCache The texture cache to use for loading any of the widgets' textures from.
             * \param customWidgetParser A custom widget parser that may be optionally passed in to override default widget parsing. 
             *
             * \return True if the view was loaded and attached to the given viewport.
             */
            bool loadViewFromYAML(Viewport* viewport, vio::IOManagerBase* iom, const cString filepath, vg::TextureCache* textureCache, WidgetParser* customWidgetParser = nullptr);
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UILoader_h__
