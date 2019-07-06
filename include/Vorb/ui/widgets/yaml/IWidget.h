//
// IWidget.h
// Vorb Engine
//
// Created by Matthew Marshall 1 Dec 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file IWidget.h
* @brief Helpful YAML parsers for IWidget.
*/

#pragma once

#ifndef Vorb_YAML_IWidget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_IWidget_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/io/Keg.h"

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing IWidget corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param widget The IWidget to be built up using the given parsed IWidget data.
         * \param name The name of the IWidget property to be parsed.
         * \param node The node of the IWidget property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the IWidget being parsed.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseIWidgetEntry(keg::ReadContext& context, vui::IWidget* widget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_IWidget_h__
