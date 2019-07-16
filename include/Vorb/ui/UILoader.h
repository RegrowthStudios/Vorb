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
            using WidgetParser = Delegate<vui::IWidget*, const nString&, keg::Node>;

            bool loadWidgetFromYAML(const vio::IOManager& iom, const cString filepath, vg::TextureCache* textureCache, Viewport* viewport, WidgetParser* customWidgetParser = nullptr);

            bool loadFromYAML(const vio::IOManager& iom, const cString filepath, vg::TextureCache* textureCache, Viewport* viewport, WidgetParser* customWidgetParser = nullptr);
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UILoader_h__
