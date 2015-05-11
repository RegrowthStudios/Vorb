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

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        // Forward Declarations
        class UIRenderer;

        class Widget : public IWidgetContainer {
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
            Widget(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Widget();

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) { /* Empty */ }

            /*! @brief Removes all drawables from the UIRenderer
            *
            * @param renderer: UIRenderer to remove from
            */
            virtual void removeDrawables(UIRenderer* renderer) { /* Empty */ }

            /*! @brief Updates the widget. Can be used for animation.
            *
            * @param dt: The TimeStep
            */
            virtual void update(f32 dt = 1.0f) { }

            /*! @brief Updates the position relative to parent */
            virtual void updatePosition() override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/ 
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const IWidgetContainer* getParent() const { return m_parent; }
            virtual const volatile bool& needsDrawableReload() const { return m_needsDrawableReload; }
            
            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setNeedsDrawableReload(bool needsDrawableReload) { m_needsDrawableReload = needsDrawableReload; }

        protected:
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            IWidgetContainer* m_parent = nullptr; ///< Parent container
            volatile bool m_needsDrawableReload = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
