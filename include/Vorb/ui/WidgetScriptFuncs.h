//
// WidgetScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file WidgetScriptFuncs.h
* @brief
* Scripts functions for Widgets
*
*/

#pragma once

#ifndef Vorb_WidgetScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/ui/Widget.h"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class Viewport;


        namespace WidgetScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                
                /***
                  From IWidget
                            ***/

                    Viewport* getViewport         (IWidget* widget);
                     IWidget* getParent           (IWidget* widget);
                // TODO(Matthew): Can we send pointers to all children?
                        f32v4 getDimensions       (IWidget* widget);
                          f32 getX                (IWidget* widget);
                          f32 getY                (IWidget* widget);
                        f32v2 getPosition         (IWidget* widget);
                        f32v2 getPaddedPosition   (IWidget* widget);
                          f32 getRelativeX        (IWidget* widget);
                          f32 getRelativeY        (IWidget* widget);
                        f32v2 getRelativePosition (IWidget* widget);
                          f32 getWidth            (IWidget* widget);
                          f32 getHeight           (IWidget* widget);
                        f32v2 getSize             (IWidget* widget);
                        f32v2 getPaddedSize       (IWidget* widget);
                        f32v4 getPadding          (IWidget* widget);
                          f32 getPaddingLeft      (IWidget* widget);
                          f32 getPaddingTop       (IWidget* widget);
                          f32 getPaddingRight     (IWidget* widget);
                          f32 getPaddingBottom    (IWidget* widget);
                // TODO(Matthew): Need to specialise ValueMediator for Clipping.
                     Clipping getClipping         (IWidget* widget);
                ClippingState getClippingLeft     (IWidget* widget);
                ClippingState getClippingTop      (IWidget* widget);
                ClippingState getClippingRight    (IWidget* widget);
                ClippingState getClippingBottom   (IWidget* widget);
                        f32v4 getClipRect         (IWidget* widget);
                       ZIndex getZIndex           (IWidget* widget);
                // TODO(Matthew): Need to specialise ValueMediator for Dock.
                         Dock getDock             (IWidget* widget);
                    DockState getDockState        (IWidget* widget);
                          f32 getDockSize         (IWidget* widget);
                      nString getName             (IWidget* widget);
                        f32v2 getChildOffset      (IWidget* widget);
                          f32 getChildOffsetX     (IWidget* widget);
                          f32 getChildOffsetY     (IWidget* widget);
                // TODO(Matthew): Need to specialise ValueMediator for WidgetFlags.
                  WidgetFlags getFlags            (IWidget* widget);


                bool isInBounds                 (IWidget* widget, f32 x, f32 y);
                bool isEnabled                  (IWidget* widget);
                bool isMouseIn                  (IWidget* widget);
                bool isClicking                 (IWidget* widget);
                bool ignoreOffset               (IWidget* widget);
                bool needsDimensionUpdate       (IWidget* widget);
                bool needsZIndexReorder         (IWidget* widget);
                bool needsDockRecalculation     (IWidget* widget);
                bool needsClipRectRecalculation (IWidget* widget);
                bool needsDrawableRecalculation (IWidget* widget);

                /***
                  From Widget
                           ***/

                PositionType getPositionType     (Widget* widget);
                     Length2 getRawPosition      (Widget* widget);
                     Length2 getRawSize          (Widget* widget);
                      Length getRawLeft          (Widget* widget);
                      Length getRawTop           (Widget* widget);
                      Length getRawRight         (Widget* widget);
                      Length getRawBottom        (Widget* widget);
                     Length2 getRawMinSize       (Widget* widget);
                     Length2 getRawMaxSize       (Widget* widget);
                      Length getRawDockSize      (Widget* widget);
                     Length4 getRawPadding       (Widget* widget);
                      Length getRawPaddingLeft   (Widget* widget);
                      Length getRawPaddingTop    (Widget* widget);
                      Length getRawPaddingRight  (Widget* widget);
                      Length getRawPaddingBottom (Widget* widget);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/

                /***
                  From IWidget
                            ***/

                bool addWidget    (IWidget* widget, IWidget* child);
                bool removeWidget (IWidget* widget, IWidget* child);

                void setX                (IWidget* widget, f32 x);
                void setY                (IWidget* widget, f32 y);
                void setPosition         (IWidget* widget, f32v2 position);
                void setRelativeX        (IWidget* widget, f32 relX);
                void setRelativeY        (IWidget* widget, f32 relY);
                void setRelativePosition (IWidget* widget, f32v2 position);
                void setWidth            (IWidget* widget, f32 width);
                void setHeight           (IWidget* widget, f32 height);
                void setSize             (IWidget* widget, f32v2 size);
                void setPadding          (IWidget* widget, f32v4 padding);
                void setPaddingLeft      (IWidget* widget, f32 left);
                void setPaddingTop       (IWidget* widget, f32 top);
                void setPaddingRight     (IWidget* widget, f32 right);
                void setPaddingBottom    (IWidget* widget, f32 bottom);
                // TODO(Matthew): Need to specialise ValueMediator for Clipping.
                void setClipping         (IWidget* widget, Clipping clipping);
                void setClippingLeft     (IWidget* widget, ClippingState state);
                void setClippingTop      (IWidget* widget, ClippingState state);
                void setClippingRight    (IWidget* widget, ClippingState state);
                void setClippingBottom   (IWidget* widget, ClippingState state);
                void setZIndex           (IWidget* widget, ZIndex zIndex);
                // TODO(Matthew): Need to specialise ValueMediator for Dock.
                void setDock             (IWidget* widget, Dock dock);
                void setDockState        (IWidget* widget, DockState state);
                void setDockSize         (IWidget* widget, f32 size);
                void setName             (IWidget* widget, nString name);
                void setChildOffset      (IWidget* widget, f32v2 offset);
                void setChildOffsetX     (IWidget* widget, f32 offset);
                void setChildOffsetY     (IWidget* widget, f32 offset);
                // TODO(Matthew): Need to specialise ValueMediator for WidgetFlags.
                void setFlags            (IWidget* widget, WidgetFlags flags);

                void setIgnoreOffset            (IWidget* widget, bool ignoreOffset);
                void setNeedsDimensionUpdate       (IWidget* widget, bool flag);
                void setNeedsZIndexReorder         (IWidget* widget, bool flag);
                void setNeedsDockRecalculation     (IWidget* widget, bool flag);
                void setNeedsClipRectRecalculation (IWidget* widget, bool flag);
                void setNeedsDrawableRecalculation (IWidget* widget, bool flag);

                /***
                  From Widget
                           ***/

                void setPositionType     (Widget* widget, PositionType positionType);
                void setRawPosition      (Widget* widget, Length2 position);
                void setRawSize          (Widget* widget, Length2 size);
                void setLeft             (Widget* widget, f32 left);
                void setTop              (Widget* widget, f32 top);
                void setRight            (Widget* widget, f32 right);
                void setBottom           (Widget* widget, f32 bottom);
                void setRawLeft          (Widget* widget, Length left);
                void setRawTop           (Widget* widget, Length top);
                void setRawRight         (Widget* widget, Length right);
                void setRawBottom        (Widget* widget, Length bottom);
                void setMinSize          (Widget* widget, f32v2 minSize);
                void setMaxSize          (Widget* widget, f32v2 maxSize);
                void setRawMinSize       (Widget* widget, Length2 minSize);
                void setRawMaxSize       (Widget* widget, Length2 maxSize);
                void setRawDockSize      (Widget* widget, Length dockSize);
                void setRawPadding       (Widget* widget, Length4 padding);
                void setRawPaddingLeft   (Widget* widget, Length left);
                void setRawPaddingTop    (Widget* widget, Length top);
                void setRawPaddingRight  (Widget* widget, Length right);
                void setRawPaddingBottom (Widget* widget, Length bottom);

                /******************************************************************/
                /* Widget Event Handlers                                          */
                /******************************************************************/
                // void onMouseClick(Sender s, const MouseButtonEvent& e);
                // void onMouseDown(Sender s, const MouseButtonEvent& e);
                // void onMouseUp(Sender s, const MouseButtonEvent& e);
                // void onMouseEnter(Sender s, const MouseMotionEvent& e);
                // void onMouseLeave(Sender s, const MouseMotionEvent& e);
                // void onMouseMove(Sender s, const MouseMotionEvent& e);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetScriptFuncs_h__
