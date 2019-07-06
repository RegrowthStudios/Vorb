//
// Slider.h
// Vorb Engine
//
// Created by Matthew Marshall 06 July 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file Slider.h
* @brief Helpful YAML parsers for Slider.
*/

#pragma once

#ifndef Vorb_YAML_Slider_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_Slider_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/io/Keg.h"

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  Slider;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing Slider corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param Slider The Slider to be built up using the given parsed Slider data.
         * \param name The name of the Slider property to be parsed.
         * \param node The node of the Slider property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the Slider being parsed.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseSliderEntry(keg::ReadContext& context, vui::Slider* button, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* cache);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_Slider_h__
