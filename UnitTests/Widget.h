//
// Widget.h
// Vorb Engine
//
// Created by Dylan Scheltens on 27 August 2015
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
#include <include/Events.hpp>
#include <include/VorbPreDecl.inl>

#include "WidgetContainer.h"

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class UIRenderer;

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

        class Widget : public WidgetContainer {
            friend class WidgetContainer;
        public:
            /*! @brief Default constructor. */
            Widget();

            /*! @brief Destructor that unhooks events */
            virtual ~Widget();

            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose() override;

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
            virtual void update(int flags, f32 dt = 1.0f) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const WidgetContainer* getParent() const { return m_parent; }
            virtual const WidgetAlign& getWidgetAlign() const { return m_align; }
            virtual const f32v2 getMinSize() const { return m_minSize; }
            virtual const f32v2 getMaxSize() const { return m_maxSize; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const UIRenderer* getRenderer() const { return m_renderer; }
            virtual const volatile bool needsDrawableReload();

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setWidgetAlign(WidgetAlign align) { m_align = align; update(0x00 | 0x01); }
            virtual void setMaxSize(const f32v2 maxSize) { m_maxSize = maxSize; update(0x00 | 0x01); }
            virtual void setMinSize(const f32v2 minSize) { m_minSize = minSize; update(0x00 | 0x01); }
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setParent(WidgetContainer* parent);
            virtual void setNeedsDrawableReload(bool needsReload);

        protected:
            f32v2 getWidgetAlignOffset(WidgetAlign align, f32v2 dimensions);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            f32v2 m_minSize = f32v2(0.0f);
            f32v2 m_maxSize = f32v2(FLT_MAX);
            WidgetAlign m_align = WidgetAlign::TOP_LEFT;
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            UIRenderer* m_renderer = nullptr; ///< For rendering.
            WidgetContainer* m_parent = nullptr; ///< Parent container
        };
    }
}

namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__

