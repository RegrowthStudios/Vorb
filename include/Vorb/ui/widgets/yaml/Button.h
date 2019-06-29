//
// Button.h
// Vorb Engine
//
// Created by Matthew Marshall 28 June 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file Button.h
* @brief Helpful YAML parsers for Button.
*/

#pragma once

#ifndef Vorb_YAML_Button_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_Button_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/io/Keg.h"

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  Button;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing Button corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param Button The Button to be built up using the given parsed Button data.
         * \param name The name of the Button property to be parsed.
         * \param node The node of the Button property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the Label being parsed.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseButtonEntry(keg::ReadContext& context, vui::Button* button, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* cache);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_Button_h__
