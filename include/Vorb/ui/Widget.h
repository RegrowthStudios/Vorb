//
// Widget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 27 Apr 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Widget.h
* @brief 
* Base class for all Widgets.
*
*/

#pragma once

#ifndef Vorb_Widget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Widget_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "Vorb/Events.hpp"
#include "Vorb/ui/IWidget.h"

namespace vorb {
    namespace ui {
        /*!
         * \brief Enum of ways a widget may be positioned.
         * 
         * Two kinds of positioning:
         *     Static   -> Raw size and positions defined by unprocessed sizes, positions;
         *     Relative -> Raw size and positions defined by unprocessed left, top, right, bottom directives.
         * Relative to three objects:
         *     Window   -> The viewport through which the player sees the game (i.e. the monitor);
         *     Canvas   -> The top-level widget of this widget's ancestors;
         *     Parent   -> The parent widget of this widget.
         */
        enum class PositionType {
            STATIC_TO_WINDOW,
            STATIC_TO_VIEWPORT,
            STATIC_TO_PARENT,
            RELATIVE_TO_WINDOW,
            RELATIVE_TO_VIEWPORT,
            RELATIVE_TO_PARENT
        };

        /*!
         * \brief Enum of ways the dimensions of a widget may be measured.
         * 
         * Dimension types are (where X is the length considered):
         *     Pixel                      -> X is directly the number of pixels;
         *     Width Percentage           -> X is the percentage of the width of the widget to which this widget is statically or relatively positioned.
         *     Height Percentage          -> X is the percentage of the height of the widget to which this widget is statically or relatively positioned.
         *     Min Percentage             -> X is the percentage of the minimum of width and height of the widget to which this widget is statically or relatively positioned.
         *     Max Percentage             -> X is the percentage of the maximum of width and height of the widget to which this widget is statically or relatively positioned.
         *     Parent Width Percentage    -> X is the percentage of the parent widget's width;
         *     Parent Height Percentage   -> X is the percentage of the parent widget's height;
         *     Parent Min Perecentage     -> X is the percentage of the parent widget's minimum of width and height;
         *     Parent Max Percentage      -> X is the percentage of the parent widget's maximum of width and height;
         *     Viewport Width Percentage  -> X is the percentage of the parent widget's width;
         *     Viewport Height Percentage -> X is the percentage of the parent widget's height;
         *     Viewport Min Perecentage   -> X is the percentage of the parent widget's minimum of width and height;
         *     Viewport Max Percentage    -> X is the percentage of the parent widget's maximum of width and height;
         *     Window Width Percentage    -> X is the percentage of the parent widget's width;
         *     Window Height Percentage   -> X is the percentage of the parent widget's height;
         *     Window Min Perecentage     -> X is the percentage of the parent widget's minimum of width and height;
         *     Window Max Percentage      -> X is the percentage of the parent widget's maximum of width and height;
         * Note that we are talking pre-normalised percentage - i.e. a value of 1.0 = 100%.
         */
        enum class DimensionType {
            PIXEL,
            WIDTH_PERCENTAGE,
            HEIGHT_PERCENTAGE,
            MIN_PERCENTAGE,
            MAX_PERCENTAGE,
            PARENT_WIDTH_PERCENTAGE,
            PARENT_HEIGHT_PERCENTAGE,
            PARENT_MIN_PERCENTAGE,
            PARENT_MAX_PERCENTAGE,
            VIEWPORT_WIDTH_PERCENTAGE,
            VIEWPORT_HEIGHT_PERCENTAGE,
            VIEWPORT_MIN_PERCENTAGE,
            VIEWPORT_MAX_PERCENTAGE,
            WINDOW_WIDTH_PERCENTAGE,
            WINDOW_HEIGHT_PERCENTAGE,
            WINDOW_MIN_PERCENTAGE,
            WINDOW_MAX_PERCENTAGE
        };

        /*!
         * \brief Type that stores a length and the dimensions of that length.
         */
        struct Length {
            f32 x;
            struct {
                DimensionType x;
            } dimension;
        };

        /*!
         * \brief Type that stores two lengths and the dimensions of those lengths.
         */
        struct Length2 {
            f32 x, y;
            struct {
                DimensionType x, y;
            } dimension;
        };

        /*!
         * \brief Type that stores four lengths and the dimensions of those lengths.
         */
        struct Length4 {
            f32 x, y, z, w;
            struct {
                DimensionType x, y, z, w;
            } dimension;
        };

        class Widget : public IWidget {
            friend class WidgetScriptFuncs;
            friend class IWidget;
        public:
            /*! \brief Default constructor. */
            Widget();
            /*! \brief Constructor that sets name, position, and dimensions.
             *
             * \param name: Name of the control.
             * \param dimensions: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(const nString& name, const f32v4& dimensions = f32v4(0.0f));
            /*! \brief Constructor that sets name, position, and dimensions.
             *
             * \param name: Name of the control.
             * \param position: The position of the widget.
             * \param size: The size of the widget.
             */
            Widget(const nString& name, const Length2& position, const Length2& size);
            /*! \brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The widget will be made a child of parent.
             *
             * \param parent: Parent container object.
             * \param name: Name of the control.
             * \param dimensions: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(IWidget* parent, const nString& name, const f32v4& dimensions = f32v4(0.0f));
            /*! \brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The widget will be made a child of parent.
             *
             * \param parent: Parent container object.
             * \param name: Name of the control.
             * \param position: The position of the widget.
             * \param size: The size of the widget.
             */
            Widget(IWidget* parent, const nString& name, const Length2& position, const Length2& size);
            /*! \brief Destructor that unhooks events */
            virtual ~Widget();

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual   PositionType getPositionType()     const { return m_positionType; }
            virtual const Length2& getRawPosition()      const { return m_rawDimensions.position; }
            virtual const Length2& getRawSize()          const { return m_rawDimensions.size; }
            virtual  const Length& getRawLeft()          const { return m_rawRelativePositions.left; }
            virtual  const Length& getRawTop()           const { return m_rawRelativePositions.top; }
            virtual  const Length& getRawRight()         const { return m_rawRelativePositions.right; }
            virtual  const Length& getRawBottom()        const { return m_rawRelativePositions.bottom; }
            virtual const Length2& getMinRawSize()       const { return m_minRawSize; }
            virtual const Length2& getMaxRawSize()       const { return m_maxRawSize; }
            virtual const Length4& getRawPadding()       const { return m_rawPadding; }
            virtual  const Length& getRawPaddingLeft()   const { return { m_rawPadding.x, { m_rawPadding.dimension.x } }; }
            virtual  const Length& getRawPaddingTop()    const { return { m_rawPadding.y, { m_rawPadding.dimension.y } }; }
            virtual  const Length& getRawPaddingRight()  const { return { m_rawPadding.z, { m_rawPadding.dimension.z } }; }
            virtual  const Length& getRawPaddingBottom() const { return { m_rawPadding.w, { m_rawPadding.dimension.w } }; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setPositionType(PositionType positionType) { m_positionType = positionType;             m_flags.needsDimensionUpdate = true; }
            virtual void setMaxRawSize(const Length2& maxRawSize)   { m_maxRawSize = maxRawSize;                 m_flags.needsDimensionUpdate = true; }
            virtual void setMaxRawSize(const f32v2& maxRawSize);
            virtual void setMinRawSize(const Length2& minRawSize)   { m_minRawSize = minRawSize;                 m_flags.needsDimensionUpdate = true; }
            virtual void setMinRawSize(const f32v2& minRawSize);
            virtual void setRawPosition(const Length2& rawPosition) { m_rawDimensions.position = rawPosition;    m_flags.needsDimensionUpdate = true; }
            virtual void setRawPosition(const f32v2& rawPosition);
            virtual void setRawSize(const Length2& rawSize)         { m_rawDimensions.size = rawSize;            m_flags.needsDimensionUpdate = true; }
            virtual void setRawSize(const f32v2& rawSize);
            virtual void setRawLeft(const Length& rawLeft)          { m_rawRelativePositions.left = rawLeft;     m_flags.needsDimensionUpdate = true; }
            virtual void setRawLeft(f32 rawLeft);
            virtual void setRawTop(const Length& rawTop)            { m_rawRelativePositions.top = rawTop;       m_flags.needsDimensionUpdate = true; }
            virtual void setRawTop(f32 rawTop);
            virtual void setRawRight(const Length& rawRight)        { m_rawRelativePositions.right = rawRight;   m_flags.needsDimensionUpdate = true; }
            virtual void setRawRight(f32 rawRight);
            virtual void setRawBottom(const Length& rawBottom)      { m_rawRelativePositions.bottom = rawBottom; m_flags.needsDimensionUpdate = true; }
            virtual void setRawBottom(f32 rawBottom);
            virtual void setRawPadding(const Length4& rawPadding)   { m_rawPadding = rawPadding;                 m_flags.needsClipRectRecalculation = true; }
            virtual void setRawPadding(const f32v4& rawPadding);
            virtual void setRawPaddingLeft(const Length& rawLeft);
            virtual void setRawPaddingLeft(f32 rawLeft);
            virtual void setRawPaddingTop(const Length& rawTop);
            virtual void setRawPaddingTop(f32 rawTop);
            virtual void setRawPaddingRight(const Length& rawRight);
            virtual void setRawPaddingRight(f32 rawRight);
            virtual void setRawPaddingBottom(const Length& rawBottom);
            virtual void setRawPaddingBottom(f32 rawBottom);
        protected:
            /*! \brief Reprocesses the pixel size and position of this widget relative to window (and parent for position). */
            virtual void updateDimensions(f32 dt) override;

            /*! \brief Calculates clipping for rendering and propagates through children. */
            virtual void calculateClipRect() override;

            /*!
             * \brief Processes a given raw length based on this widget's ancestors.
             * 
             * \param length: The raw length to be processed.
             * 
             * \returns The processed length.
             */
            f32 processLength(Length length);
            /*!
             * \brief Processes a given raw length based on this widget's ancestors.
             * 
             * \param length: The raw length to be processed.
             * 
             * \returns The processed length.
             */
            f32v2 processLength(Length2 length);

            /*!
             * \brief Applies the minimum and maximum sizes specified for the widget.
             */
            void applyMinMaxSizes();

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            PositionType m_positionType = PositionType::STATIC_TO_PARENT;                                       ///< The type of positioning this widget uses.
            Length2      m_minRawSize   = { 0.0f, 0.0f, { DimensionType::PIXEL, DimensionType::PIXEL } };       ///< Minimum size of widget.
            Length2      m_maxRawSize   = { FLT_MAX, FLT_MAX, { DimensionType::PIXEL, DimensionType::PIXEL } }; ///< Maximum size of widget.

            union {
                struct {
                    Length2 position; ///< Position of widget in specified dimensions.
                    Length2 size;     ///< Dimensions of widget in specified dimensions.
                } m_rawDimensions = { { 0.0f, 0.0f, { DimensionType::PIXEL, DimensionType::PIXEL } }, { 0.0, 0.0, { DimensionType::PIXEL, DimensionType::PIXEL } } };
                struct {
                    Length left;   ///< Position of widget relative to left of target widget.
                    Length top;    ///< Position of widget relative to top of target widget.
                    Length right;  ///< Position of widget relative to right of target widget.
                    Length bottom; ///< Position of widget relative to bottom of target widget.
                } m_rawRelativePositions;
            };

            Length4 m_rawPadding   = { 0.0f, 0.0f, 0.0f, 0.0f, { DimensionType::PIXEL, DimensionType::PIXEL, DimensionType::PIXEL, DimensionType::PIXEL } }; ///< Padding of widget.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
