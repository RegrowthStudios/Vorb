//
// Widget.h
// Vorb Engine
//
// Created by Matthew Marshall 1 Dec 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file Widget.h
* @brief Helpful YAML parsers for Widget.
*/

#pragma once

#ifndef Vorb_YAML_Widget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_Widget_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/io/Keg.h"

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  Widget;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing Widget corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param widget The widget to be built up using the given parsed widget data.
         * \param name The name of the widget property to be parsed.
         * \param node The node of the Widget property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the widget being parsed.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseWidgetEntry(keg::ReadContext& context, vui::Widget* widget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_Widget_h__
