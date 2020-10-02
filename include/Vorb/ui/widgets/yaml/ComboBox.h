//
// ComboBox.h
// Vorb Engine
//
// Created by Matthew Marshall 28 June 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file ComboBox.h
* @brief Helpful YAML parsers for ComboBox.
*/

#pragma once

#ifndef Vorb_YAML_ComboBox_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_ComboBox_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Keg.h"

DECL_VG(class FontCache; class TextureCache);

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  ComboBox;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing ComboBox corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param ComboBox The ComboBox to be built up using the given parsed ComboBox data.
         * \param name The name of the ComboBox property to be parsed.
         * \param node The node of the ComboBox property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the ComboBox being parsed.
         * \param fontCache The font cache to use for loading any of the widget's fonts from.
         * \param textureCache The texture cache to use for loading any of the widget's textures from.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseComboBoxEntry(keg::ReadContext& context, ComboBox* comboBox, const nString& name, keg::Node value, Delegate<IWidget*, const nString&, keg::Node>* widgetParser, vg::FontCache* fontCache, vg::TextureCache* textureCache);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_ComboBox_h__
