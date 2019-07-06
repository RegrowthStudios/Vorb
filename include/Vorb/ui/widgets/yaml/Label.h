//
// Label.h
// Vorb Engine
//
// Created by Matthew Marshall 1 Dec 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file Label.h
* @brief Helpful YAML parsers for Label.
*/

#pragma once

#ifndef Vorb_YAML_Label_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAML_Label_h__
//! @endcond

#include "Vorb/types.h"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/io/Keg.h"

DECL_VG(class TextureCache);

namespace vorb {
    namespace ui {
        // Forward Declarations.
        class  Label;
        class  IWidget;

        /*!
         * \brief Parses the entry of the currently-parsing Label corresponding to the given YAML node.
         *
         * \param context The YAML context to use for reading nodes.
         * \param label The Label to be built up using the given parsed Label data.
         * \param name The name of the Label property to be parsed.
         * \param node The node of the Label property to be parsed.
         * \param widgetParser Marshals parsing of the widgets that are children of the Label being parsed.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseLabelEntry(keg::ReadContext& context, Label* label, const nString& name, keg::Node value, Delegate<IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* cache);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_YAML_Label_h__
