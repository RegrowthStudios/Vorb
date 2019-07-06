//
// CheckBox.h
// Vorb Engine
//
// Created by Matthew Marshall 28 June 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file CheckBox.h
* @brief Helpful YAML parsers for CheckBox.
*/

#pragma once

#ifndef Vorb_YAML_CheckBox_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_CheckBox_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/io/Keg.h"

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  CheckBox;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing CheckBox corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param CheckBox The CheckBox to be built up using the given parsed CheckBox data.
         * \param name The name of the CheckBox property to be parsed.
         * \param node The node of the CheckBox property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the CheckBox being parsed.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseCheckBoxEntry(keg::ReadContext& context, vui::CheckBox* checkBox, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* cache);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_CheckBox_h__
