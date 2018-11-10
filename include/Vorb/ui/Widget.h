//
// Widget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 27 Apr 2015
// Copyright 2014 Regrowth Studios
// MIT License
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

#include "Vorb/Event.hpp"
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
        public:
            /*! \brief Default constructor. */
            Widget();
            /*! \brief Destructor that unhooks events */
            virtual ~Widget();

            /*! \brief Initialiser that sets name, position, size, and Z-index.
            *
            * \param name: Name of the widget.
            * \param dimensions: Position and size of widget.
            * \param zIndex: Z-index of widget.
            */
            virtual void init(const nString& name, const f32v4& dimensions = f32v4(0.0f), ui16 zIndex = 0) override;
            /*! \brief Initialiser that sets name, position, size, and Z-index.
             *
            * \param name: Name of the widget.
            * \param position: Position of widget.
            * \param size: Size of widget.
            * \param zIndex: Z-index of widget.
             */
            virtual void init(const nString& name, const Length2& position, const Length2& size, ui16 zIndex = 0);
            /*! \brief Initialiser that sets parent, name, position, size, and Z-index.
            *
            * \param parent: Parent of the widget.
            * \param name: Name of the widget.
            * \param dimensions: Position and size of widget.
            * \param zIndex: Z-index of widget.
            */
            virtual void init(IWidget* parent, const nString& name, const f32v4& dimensions = f32v4(0.0f), ui16 zIndex = 0) override;
            /*! \brief Initialiser that sets parent, name, position, size, and Z-index.
             *
             * \param parent: Parent of the widget.
             * \param name: Name of the widget.
             * \param position: Position of widget.
             * \param size: Size of widget.
             * \param zIndex: Z-index of widget.
             */
            virtual void init(IWidget* parent, const nString& name, const Length2& position, const Length2& size, ui16 zIndex = 0);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual   PositionType getPositionType()     const { return m_positionType;                }
            virtual const Length2& getRawPosition()      const { return m_rawDimensions.position;      }
            virtual const Length2& getRawSize()          const { return m_rawDimensions.size;          }
            virtual  const Length& getRawLeft()          const { return m_rawRelativePositions.left;   }
            virtual  const Length& getRawTop()           const { return m_rawRelativePositions.top;    }
            virtual  const Length& getRawRight()         const { return m_rawRelativePositions.right;  }
            virtual  const Length& getRawBottom()        const { return m_rawRelativePositions.bottom; }
            virtual const Length2& getMinRawSize()       const { return m_minRawSize;                     }
            virtual const Length2& getMaxRawSize()       const { return m_maxRawSize;                     }
            virtual  const Length& getRawDockSize()      const { return m_rawDockSize;                 }
            virtual            f32 getDockSize()         const override { return processLength(m_rawDockSize); }
            virtual const Length4& getRawPadding()       const { return m_rawPadding;                  }
            virtual         Length getRawPaddingLeft()   const { return { m_rawPadding.x, { m_rawPadding.dimension.x } }; }
            virtual         Length getRawPaddingTop()    const { return { m_rawPadding.y, { m_rawPadding.dimension.y } }; }
            virtual         Length getRawPaddingRight()  const { return { m_rawPadding.z, { m_rawPadding.dimension.z } }; }
            virtual         Length getRawPaddingBottom() const { return { m_rawPadding.w, { m_rawPadding.dimension.w } }; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setPositionType(PositionType posType)   { m_positionType = posType; m_flags.needsDimensionUpdate = true; }
            virtual void setRawMaxSize(const Length2& maxSize)   { m_maxRawSize = maxSize;   m_flags.needsDimensionUpdate = true; }
            virtual void setMaxSize(const f32v2& maxSize);
            virtual void setRawMinSize(const Length2& minSize)   { m_minRawSize = minSize;   m_flags.needsDimensionUpdate = true; }
            virtual void setMinSize(const f32v2& minSize);
            virtual void setRawDockSize(const Length& dockSize);
            virtual void setDockSize(f32 dockSize) override;
            virtual void setRawPosition(const Length2& position) { m_rawDimensions.position = position;    m_flags.needsDimensionUpdate = true; }
            virtual void setPosition(f32v2 position) override;
            virtual void setX(f32 x) override;
            virtual void setY(f32 y) override;
            virtual void setRawSize(const Length2& size)         { m_rawDimensions.size = size;            m_flags.needsDimensionUpdate = true; }
            virtual void setSize(f32v2 size) override;
            virtual void setWidth(f32 width) override;
            virtual void setHeight(f32 height) override;
            virtual void setRawLeft(const Length& left)          { m_rawRelativePositions.left = left;     m_flags.needsDimensionUpdate = true; }
            virtual void setLeft(f32 left);
            virtual void setRawTop(const Length& top)            { m_rawRelativePositions.top = top;       m_flags.needsDimensionUpdate = true; }
            virtual void setTop(f32 top);
            virtual void setRawRight(const Length& right)        { m_rawRelativePositions.right = right;   m_flags.needsDimensionUpdate = true; }
            virtual void setRight(f32 right);
            virtual void setRawBottom(const Length& bottom)      { m_rawRelativePositions.bottom = bottom; m_flags.needsDimensionUpdate = true; }
            virtual void setBottom(f32 bottom);
            virtual void setRawPadding(const Length4& padding)   { m_rawPadding = padding;                 m_flags.needsDimensionUpdate = true; }
            virtual void setPadding(const f32v4& padding) override;
            virtual void setRawPaddingLeft(const Length& left);
            virtual void setPaddingLeft(f32 left) override;
            virtual void setRawPaddingTop(const Length& top);
            virtual void setPaddingTop(f32 top) override;
            virtual void setRawPaddingRight(const Length& right);
            virtual void setPaddingRight(f32 right) override;
            virtual void setRawPaddingBottom(const Length& bottom);
            virtual void setPaddingBottom(f32 bottom) override;
        protected:
            /*! \brief Reprocesses the pixel size and position of this widget relative to window (and parent for position). */
            virtual void updateDimensions(f32) override;

            /*!
             * \brief Processes a given raw length based on this widget's ancestors.
             * 
             * \param length: The raw length to be processed.
             * 
             * \returns The processed length.
             */
            f32 processLength(Length length) const;
            /*!
             * \brief Processes a given raw length based on this widget's ancestors.
             * 
             * \param length: The raw length to be processed.
             * 
             * \returns The processed length.
             */
            f32v2 processLength(Length2 length) const;

            /*!
             * \brief Applies the minimum and maximum sizes specified for the widget.
             */
            void applyMinMaxSizes();

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            PositionType m_positionType; ///< The type of positioning this widget uses.
            Length2      m_minRawSize;   ///< Minimum size of widget.
            Length2      m_maxRawSize;   ///< Maximum size of widget.
            Length       m_rawDockSize;  ///< The raw dock size of widget.
            Length4      m_rawPadding;   ///< Padding of widget.

            union {
                struct {
                    Length2 position; ///< Position of widget in specified dimensions.
                    Length2 size;     ///< Dimensions of widget in specified dimensions.
                } m_rawDimensions;
                struct {
                    Length left;   ///< Position of widget relative to left of target widget.
                    Length top;    ///< Position of widget relative to top of target widget.
                    Length right;  ///< Position of widget relative to right of target widget.
                    Length bottom; ///< Position of widget relative to bottom of target widget.
                } m_rawRelativePositions;
            };
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
