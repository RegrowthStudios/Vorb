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
#include "types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "../Events.hpp"
#include "../VorbPreDecl.inl"
#include "IWidgetContainer.h"
#include "Form.h"
#include "../math/TweeningMath.hpp"

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        //! Enum of widget alignments.
        enum class WidgetAlign {
            LEFT,
            TOP_LEFT,
            TOP,
            TOP_RIGHT,
            RIGHT,
            BOTTOM_RIGHT,
            BOTTOM,
            BOTTOM_LEFT,
            CENTER
        };

        //! Enum of position types.
        enum class PositionType {
            STATIC,
            ABSOLUTE,
            FIXED,
            RELATIVE
        };

        //! Enum of docking styles.
        enum class DockingStyle {
            NONE,
            LEFT,
            TOP,
            RIGHT,
            BOTTOM,
            FILL
        };

        //! Struct of docking options.
        struct DockingOptions {
            DockingStyle style;
            Length rawSize;
            f32 size;
        };
        
        // TODO(Matthew): Optional callback on transition completion?
        //! Structs of target length and time frame to completion.
        struct UITransition : public vmath::Transition32_F32 {
            Length rawInitialLength, rawFinalLength;

            UITransition() {
                rawInitialLength = { 0.0f, { UnitType::PIXEL } };
                rawFinalLength = { 0.0f, { UnitType::PIXEL } };
                initialLength = 0.0f;
                finalLength = 0.0f;
                currentTime = 0.0f;
                finalTime = 0.0f;
                tweeningFunc = nullptr;
            }

            UITransition(const Length& _rawInitialLength, const Length& _rawFinalLength, f32 duration, f32(*_tweeningFunc)(f32, f32, f32)) {
                rawInitialLength = _rawInitialLength;
                rawFinalLength = _rawFinalLength;
                initialLength = 0.0f;
                finalLength = 0.0f;
                currentTime = 0.0f;
                finalTime = duration;
                tweeningFunc = _tweeningFunc;
            }
        };
        struct UITransition2 : public vmath::Transition32v2_F32 {
            Length2 rawInitialLength, rawFinalLength;

            UITransition2() {
                rawInitialLength = { 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } };
                rawFinalLength = { 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } };
                initialLength = f32v2(0.0f);
                finalLength = f32v2(0.0f);
                currentTime = 0.0f;
                finalTime = 0.0f;
                tweeningFunc = nullptr;
            }

            UITransition2(const Length2& _rawInitialLength, const Length2& _rawFinalLength, f32 duration, f32v2(*_tweeningFunc)(f32v2, f32v2, f32)) {
                rawInitialLength = _rawInitialLength;
                rawFinalLength = _rawFinalLength;
                initialLength = 0.0f;
                finalLength = 0.0f;
                currentTime = 0.0f;
                finalTime = duration;
                tweeningFunc = _tweeningFunc;
            }
        };

        // Forward Declarations
        class UIRenderer;

        class Widget : public IWidgetContainer {
            friend class WidgetScriptFuncs;
            friend class IWidgetContainer;
        public:
            /*! @brief Default constructor. */
            Widget();
            /*! @brief Constructor that sets name, position, and dimensions.
             *
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The widget will be made a child of parent.
            *
            * @param parent: Parent container object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Widget(Form* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The widget will be made a child of parent.
             *
             * @param parent: Parent container object.
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(Widget* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Widget();
            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose() override;

            /*! @brief Adds a widget to the Form and initializes it for rendering.
            *
            * @param widget: The Widget to add.
            * @return true on success.
            */
            virtual bool addWidget(Widget* widget);

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to.
            */
            virtual void addDrawables(UIRenderer* renderer);

            /*! @brief Removes all drawables from the UIRenderer */
            virtual void removeDrawables();

            /*! @brief Updates the widget. Can be used for animation.
            *
            * @param dt: The TimeStep
            */
            virtual void update(f32 dt = 0.0f);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/ 
            Form* getParentForm() const { return m_parentForm; }
            Widget* getParentWidget() const { return m_parentWidget; }
            const IWidgetContainer* getFirstPositionedParent() const;
            const DockingOptions& getDockingOptions() const { return m_dockingOptions; } // TODO(Matthew): Reimplement docking in new update system.
            f32 getProcessedDockingSize() const { return m_processedDockingSize; }
            const AnchorStyle& getAnchor() const { return m_anchor; }
            const PositionType& getPositionType() const { return m_positionType; }
            const volatile bool& needsDrawableReload() const { return m_needsDrawableReload; }
            const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            const UIRenderer* getRenderer() const { return m_renderer; }
            const Length2& getRawPosition() const { return m_rawPosition; }
            const UITransition2& getTargetPosition() const { return m_targetPosition; }
            const f32v2& getRelativePosition() const { return m_relativePosition; }
            const Length2& getRawDimensions() const { return m_rawDimensions; }
            const UITransition2& getTargetDimensions() const { return m_targetDimensions; }
            const Length2& getRawMinSize() const { return m_rawMinSize; }
            const UITransition2& getTargetMinSize() const { return m_targetMinSize; }
            const f32v2& getMinSize() const { return m_minSize; }
            const Length2& getRawMaxSize() const { return m_rawMaxSize; }
            const UITransition2& getTargetMaxSize() const { return m_targetMaxSize; }
            const f32v2& getMaxSize() const { return m_maxSize; }
            //const WidgetAlign& getWidgetAlign() const { return m_align; }
            //ui16 getZIndex() const { return m_zIndex; }

            bool isPositionTransitioning() const { return m_targetPosition.currentTime < m_targetPosition.finalTime; }
            bool isDimensionsTransitioning() const { return m_targetDimensions.currentTime < m_targetDimensions.finalTime; }
            bool isMaxSizeTransitioning() const { return m_targetMaxSize.currentTime < m_targetMaxSize.finalTime; }
            bool isMinSizeTransitioning() const { return m_targetMinSize.currentTime < m_targetMinSize.finalTime; }
            bool isDockingTransitioning() const { return m_targetDockingSize.currentTime < m_targetDockingSize.finalTime; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setAnchor(const AnchorStyle& anchor) { m_anchor = anchor; }
            //virtual void setDockingOptions(const DockingOptions& options);
            virtual void setRawDockingSize(const Length& size) { m_dockingOptions.rawSize = size; m_pendingUpdates |= UpdateFlag::RAW_DOCKING_SIZE; }
            //virtual void setRawTargetDockingSize(const Transition& targetSize); // TODO(Matthew): Implement.
            //virtual void setDockingStyle(const DockingStyle& style);
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setNeedsDrawableReload(bool needsDrawableReload) { m_needsDrawableReload = needsDrawableReload; }
            virtual void setPositionType(const PositionType& positionType) { m_positionType = positionType; m_pendingUpdates |= UpdateFlag::SPATIAL; }
            virtual void setPosition(const f32v2& position) override { m_rawPosition = { position.x, position.y, { UnitType::PIXEL, UnitType::PIXEL } }; IWidgetContainer::setPosition(position); }
            virtual void setRawPosition(const Length2& rawPosition) { m_rawPosition = rawPosition; m_pendingUpdates |= UpdateFlag::RAW_POSITION; }
            virtual void setRawPosition(const f32v2& rawPosition, UnitType& units) { m_rawPosition = { rawPosition.x, rawPosition.y, { units, units } }; m_pendingUpdates |= UpdateFlag::RAW_POSITION; }
            virtual void setRawPositionX(f32 value, UnitType& units) { m_rawPosition.x = value; m_rawPosition.units.x = units; m_pendingUpdates |= UpdateFlag::RAW_POSITION; }
            virtual void setRawPositionY(f32 value, UnitType& units) { m_rawPosition.y = value; m_rawPosition.units.y = units; m_pendingUpdates |= UpdateFlag::RAW_POSITION; }
            virtual void setTargetPosition(const UITransition2& targetPosition) { m_targetPosition = targetPosition;  m_pendingUpdates |= UpdateFlag::TRANSITION_POSITION; } // TODO(Matthew): Some more friendly setting functions?
            virtual void setDimensions(const f32v2& dimensions) override { m_rawDimensions = { dimensions.x, dimensions.y, { UnitType::PIXEL, UnitType::PIXEL } }; IWidgetContainer::setDimensions(dimensions); }
            virtual void setHeight(f32 height) override { m_rawDimensions = { m_rawDimensions.x, height, { m_rawDimensions.units.x, UnitType::PIXEL } }; IWidgetContainer::setHeight(height); }
            virtual void setWidth(f32 width) override { m_rawDimensions = { width, m_rawDimensions.y, { UnitType::PIXEL, m_rawDimensions.units.y } }; IWidgetContainer::setWidth(width); }
            virtual void setRawDimensions(const Length2& rawDimensions) { m_rawDimensions = rawDimensions; m_pendingUpdates |= UpdateFlag::RAW_DIMENSIONS; }
            virtual void setRawDimensions(const f32v2& rawDimensions, UnitType& units) { m_rawDimensions = { rawDimensions.x, rawDimensions.y, { units, units } }; m_pendingUpdates |= UpdateFlag::RAW_DIMENSIONS; }
            virtual void setRawWidth(f32 value, UnitType& units) { m_rawDimensions.x = value; m_rawDimensions.units.x = units; m_pendingUpdates |= UpdateFlag::RAW_DIMENSIONS; }
            virtual void setRawHeight(f32 value, UnitType& units) { m_rawDimensions.y = value; m_rawDimensions.units.y = units; m_pendingUpdates |= UpdateFlag::RAW_DIMENSIONS; }
            virtual void setTargetDimensions(const UITransition2& targetDimensions) { m_targetDimensions = targetDimensions; m_pendingUpdates |= UpdateFlag::TRANSITION_DIMENSIONS; }
            virtual void setRawMaxSize(const Length2& maxSize) { m_rawMaxSize = maxSize; m_pendingUpdates |= UpdateFlag::RAW_MAX_SIZE; }
            virtual void setRawMaxSize(const f32v2& maxSize, UnitType& units) { m_rawMaxSize = { maxSize.x, maxSize.y, { units, units } }; m_pendingUpdates |= UpdateFlag::RAW_MAX_SIZE; }
            virtual void setRawMaxWidth(f32 maxWidth, UnitType& units) { m_rawMaxSize.x = maxWidth; m_rawMaxSize.units.x = units; m_pendingUpdates |= UpdateFlag::RAW_MAX_SIZE; }
            virtual void setRawMaxHeight(f32 maxHeight, UnitType& units) { m_rawMaxSize.y = maxHeight; m_rawMaxSize.units.y = units; m_pendingUpdates |= UpdateFlag::RAW_MAX_SIZE; }
            virtual void setMaxSize(const f32v2& maxSize);
            virtual void setTargetMaxSize(const UITransition2& targetMaxSize, bool update = true) { m_targetMaxSize = targetMaxSize; m_pendingUpdates |= UpdateFlag::TRANSITION_MAX_SIZE; }
            virtual void setRawMinSize(const Length2& minSize) { m_rawMinSize = minSize; m_pendingUpdates |= UpdateFlag::RAW_MIN_SIZE; }
            virtual void setRawMinSize(const f32v2& minSize, UnitType& units) { m_rawMinSize = { minSize.x, minSize.y, { units, units } }; m_pendingUpdates |= UpdateFlag::RAW_MIN_SIZE; }
            virtual void setRawMinWidth(f32 minWidth, UnitType& units) { m_rawMinSize.x = minWidth; m_rawMinSize.units.x = units; m_pendingUpdates |= UpdateFlag::RAW_MIN_SIZE; }
            virtual void setRawMinHeight(f32 minHeight, UnitType& units) { m_rawMinSize.y = minHeight; m_rawMinSize.units.y = units; m_pendingUpdates |= UpdateFlag::RAW_MIN_SIZE; }
            virtual void setMinSize(const f32v2& minSize);
            virtual void setTargetMinSize(const UITransition2& targetMinSize, bool update = true) { m_targetMinSize = targetMinSize; m_pendingUpdates |= UpdateFlag::TRANSITION_MIN_SIZE; }
            //virtual void setWidgetAlign(WidgetAlign align) { m_align = align; m_pendingUpdates |= UpdateFlag::POSITION; }
            //virtual void setZIndex(ui16 zIndex);
            
        protected:
            /*! @brief Sets the parent Form for this container.
            *
            * @param parent: The parent Form to be set for this Widget.
            */
            void setParentForm(Form* parent);
            /*! @brief Sets the parent Widget for this container.
            *
            * @param parent: The parent Widget to be set for this Widget.
            */
            void setParentWidget(Widget* parent);

            //virtual f32v2 getWidgetAlignOffset();

            /*! @brief Processes the raw position relative to parent */
            virtual void processRawPosition();
            /*! @brief Processes the raw dimensions of the widget. */
            virtual void processRawDimensions();
            /*! @brief Processes the raw max size of the widget. */
            virtual void processRawMaxSize();
            /*! @brief Processes the raw min size of the widget. */
            virtual void processRawMinSize();
            /*! @brief Processes the raw size of the docking of this widget. */
            virtual void processRawDockingSize();
            /*! @brief Processes the raw transitioning positions of the widget. */
            virtual void processRawPositionTransition();
            /*! @brief Processes the raw transitioning dimensions of the widget. */
            virtual void processRawDimensionsTransition();
            /*! @brief Processes the raw transitioning max sizes of the widget. */
            virtual void processRawMaxSizeTransition();
            /*! @brief Processes the raw transitioning min sizes of the widget. */
            virtual void processRawMinSizeTransition();
            /*! @brief Processes the raw transitioning docking sizes of the widget. */
            virtual void processRawDockingTransition();

            /*! @brief Processes the position transition of the widget. */
            virtual void processPositionTransition(f32 dt);
            /*! @brief Processes the dimensions transition of the widget. */
            virtual void processDimensionsTransition(f32 dt);
            /*! @brief Processes the max size transition of the widget. */
            virtual void processMaxSizeTransition(f32 dt);
            /*! @brief Processes the min size transition of the widget. */
            virtual void processMinSizeTransition(f32 dt);
            /*! @brief Processes the docking transition of the widget. */
            virtual void processDockingTransition(f32 dt);

            /*! @brief Recalculates clipping. */
            virtual void recalculateClipping();
            /*! @brief Recalculates text. */
            virtual void recalculateText() = 0;
            /*! @brief Recalculates docking of children. */
            virtual void recalculateDocking();
            /*! @brief Recalculates the drawables of the children. */
            virtual void recalculateDrawables() = 0;
            /*! @brief Recalculates drawing order of children. */
            virtual void recalculateDrawableOrder();

            /*! @brief Allows custom behaviour when position or dimensions change. */
            virtual void onSpatialUpdate();
            /*! @brief Allows custom behaviour when the position transition changes. */
            virtual void onPositionTransitionUpdate();
            /*! @brief Allows custom behaviour when the dimensions transition changes. */
            virtual void onDimensionsTransitionUpdate();
            /*! @brief Allows custom behaviour when the max size transition changes. */
            virtual void onMaxSizeTransitionUpdate();
            /*! @brief Allows custom behaviour when the min size transition changes. */
            virtual void onMinSizeTransitionUpdate();
            /*! @brief Allows custom behaviour when the docking transition changes. */
            virtual void onDockingTransitionUpdate();
            /*! @brief Allows custom behaviour when the docking size changes. */
            virtual void onDockingSizeUpdate();
            /*! @brief Allows custom behaviour when the docking style changes. */
            virtual void onDockingStyleUpdate();

            /*! @brief Refreshes drawables. */
            virtual void refreshDrawables();

            /*! @brief Computes clipping for this widget container. */
            virtual void computeClipRect() override;

            /*! @brief Processes a set of raw values and converts them to processed values that can be used for basic calculations. */
            virtual f32v2 processRawValues(const Length2& rawValues);
            virtual f32 processRawValues(const Length& rawValues);
            virtual f32v2 processRawValue(const f32v2& rawValue, const UnitType& unit);

            /*! @brief Calculate relatve-to-parent change in position. */
            virtual f32v2 calculateRelativeToParentShift();

            /*! @brief Applies the current min and max sizes to the provided dimensions. */
            virtual void applyMinMaxSizesToDimensions(f32v2& dimensions);

            /*! @broef Adds an update flag to the pending updates for this element's most direct parent. */
            void applyUpdateFlagToParent(ui32 flag);
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            Form* m_parentForm = nullptr; ///< Parent form.
            Widget* m_parentWidget = nullptr; ///< Parent widget.
            WidgetAlign m_align = WidgetAlign::TOP_LEFT; ///< Direction of alignment of the widget.
            AnchorStyle m_anchor; ///< The anchor data.
            DockingOptions m_dockingOptions; ///< Docking options of the widget.
            // TODO(Matthew): Move transitions in some way to IWidgetContainer, at least in processed position/dimension so form may transition.
            UITransition m_targetDockingSize; ///< The target docking size of the widget.
            f32 m_processedDockingSize; ///< Cache of processed docking size.
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            UIRenderer* m_renderer = nullptr; ///< The renderer to be used by the widget.
            ui16 m_zIndex; ///< The Z-index of the widget.
            PositionType m_positionType = PositionType::STATIC; ///< The type of positioning of the widget.
            Length2 m_rawPosition; ///< The raw position of the widget.
            UITransition2 m_targetPosition; ///< The target raw position of the widget.
            // TODO(Matthew): Do we need this?
            f32v2 m_relativePosition = f32v2(0.0f); ///< The relative position of the widget.
            Length2 m_rawDimensions; ///< The raw dimensions of the widget.
            UITransition2 m_targetDimensions; ///< The target raw dimensions of the widget.
            Length2 m_rawMinSize; ///< The raw minimum dimensions of the widget.
            UITransition2 m_targetMinSize; ///< The target raw minimum dimensions of the widget.
            f32v2 m_minSize = f32v2(0.0f); ///< The processed minimum dimensions of the widget.
            Length2 m_rawMaxSize; ///< The raw maximum dimensions of the widget.
            UITransition2 m_targetMaxSize; ///< The target raw minimum dimensions of the widget.
            f32v2 m_maxSize = f32v2(FLT_MAX); ///< The processed maximum dimensions of the widget.
            volatile bool m_needsDrawableReload = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
