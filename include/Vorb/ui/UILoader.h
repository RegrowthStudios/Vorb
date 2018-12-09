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

#include "Vorb/types.h"
#include "Vorb/VorbPreDecl.inl"

DECL_VIO(class IOManager)
DECL_VUI(class Viewport)

namespace vorb {
    namespace ui {
        namespace UILoader {
            using WidgetParser = Delegate<vui::IWidget*, const nString&, keg::Node>;

            bool load(const vio::IOManager& iom, const cString filepath, OUT Viewport& viewport, WidgetParser* customWidgetParser = nullptr);

            bool save(const cString filepath, const Viewport& viewport);
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UILoader_h__
