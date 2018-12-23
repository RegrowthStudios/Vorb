//
// TextWidget.h
// Vorb Engine
//
// Created by Matthew Marshall 1 Dec 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file TextWidget.h
* @brief Helpful YAML parsers for TextWidget.
*/

#pragma once

#ifndef Vorb_YAML_TextWidget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_TextWidget_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Keg.h"

DECL_VG(class TextureCache)

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  TextWidget;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing TextWidget corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param label The TextWidget to be built up using the given parsed TextWidget data.
         * \param name The name of the TextWidget property to be parsed.
         * \param node The node of the TextWidget property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the TextWidget being parsed.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseTextWidgetEntry(keg::ReadContext& context, vui::TextWidget* label, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_TextWidget_h__
