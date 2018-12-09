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
        enum PositionType;
        enum DimensionType;
        struct Length;
        struct Length2;
        struct Length4;
        class  Widget;
        class  IWidget;

        /*!
         * \brief Parses the clipping state corresponding to the given name.
         *
         * \param name The name of the clipping state to be parsed.
         * \param clippingState The output state if parsed successfully.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseClippingState(const nString& name, OUT vui::ClippingState& clippingState);

        /*!
         * \brief Parses the dock state corresponding to the given name.
         *
         * \param name The name of the dock state to be parsed.
         * \param dockState The output state if parsed successfully.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseDockState(const nString& name, OUT vui::DockState& dockState);

        /*!
         * \brief Parses the position type corresponding to the given name.
         *
         * \param name The name of the position type to be parsed.
         * \param positionType The output position type if parsed successfully.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parsePositionType(const nString& name, OUT vui::PositionType& positionType);

        /*!
         * \brief Parses the dimensions type corresponding to the given name.
         *
         * \param name The name of the dimensions type to be parsed.
         * \param dimensionsType The output dimensions type if parsed successfully.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseDimensionType(const nString& name, OUT vui::DimensionType& dimensionType);

        /*!
         * \brief Parses the length corresponding to the given YAML node.
         *
         * \param node The node of the length to be parsed.
         * \param length The output length if parsed successfully.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseLength(keg::Node node, OUT vui::Length& length);

        /*!
         * \brief Parses the 2D length corresponding to the given YAML node.
         *
         * \param node The node of the length to be parsed.
         * \param length The output length if parsed successfully.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseLength2(keg::Node node, OUT vui::Length2& length);

        /*!
         * \brief Parses the 4D length corresponding to the given YAML node.
         *
         * \param node The node of the length to be parsed.
         * \param length The output length if parsed successfully.
         *
         * \return True if parsing is successful, false otherwise.
         */
        bool parseLength4(keg::Node node, OUT vui::Length4& length);

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
