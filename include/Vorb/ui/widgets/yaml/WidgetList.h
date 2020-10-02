//
// WidgetList.h
// Vorb Engine
//
// Created by Matthew Marshall 1 Dec 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file WidgetList.h
* @brief Helpful YAML parsers for WidgetList.
*/

#pragma once

#ifndef Vorb_YAML_WidgetList_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_WidgetList_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Keg.h"

DECL_VG(class TextureCache);

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  WidgetList;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing WidgetList corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param label The WidgetList to be built up using the given parsed WidgetList data.
         * \param name The name of the WidgetList property to be parsed.
         * \param node The node of the WidgetList property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the WidgetList being parsed.
         * \param textureCache The texture cache to use for loading any of the widget's textures from.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseWidgetListEntry(keg::ReadContext& context, WidgetList* widgetList, const nString& name, keg::Node value, Delegate<IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_WidgetList_h__
