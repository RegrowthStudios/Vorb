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

#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/widgets/Widget.h"

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
                     Clipping getClipping         (IWidget* widget);
                ClippingState getClippingLeft     (IWidget* widget);
                ClippingState getClippingTop      (IWidget* widget);
                ClippingState getClippingRight    (IWidget* widget);
                ClippingState getClippingBottom   (IWidget* widget);
                        f32v4 getClipRect         (IWidget* widget);
                       ZIndex getZIndex           (IWidget* widget);
                         Dock getDock             (IWidget* widget);
                    DockState getDockState        (IWidget* widget);
                          f32 getDockSize         (IWidget* widget);
                      nString getName             (IWidget* widget);
                        f32v2 getChildOffset      (IWidget* widget);
                          f32 getChildOffsetX     (IWidget* widget);
                          f32 getChildOffsetY     (IWidget* widget);
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
                void setClipping         (IWidget* widget, Clipping clipping);
                void setClippingLeft     (IWidget* widget, ClippingState state);
                void setClippingTop      (IWidget* widget, ClippingState state);
                void setClippingRight    (IWidget* widget, ClippingState state);
                void setClippingBottom   (IWidget* widget, ClippingState state);
                void setZIndex           (IWidget* widget, ZIndex zIndex);
                void setDock             (IWidget* widget, Dock dock);
                void setDockState        (IWidget* widget, DockState state);
                void setDockSize         (IWidget* widget, f32 size);
                void setName             (IWidget* widget, nString name);
                void setChildOffset      (IWidget* widget, f32v2 offset);
                void setChildOffsetX     (IWidget* widget, f32 offset);
                void setChildOffsetY     (IWidget* widget, f32 offset);
                void setFlags            (IWidget* widget, WidgetFlags flags);

                void setIgnoreOffset               (IWidget* widget, bool ignoreOffset);
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
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::WidgetScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getViewport",         makeDelegate(&impl::getViewport));
    env->addCDelegate("getParent",           makeDelegate(&impl::getParent));
    env->addCDelegate("getDimensions",       makeDelegate(&impl::getDimensions));
    env->addCDelegate("getX",                makeDelegate(&impl::getX));
    env->addCDelegate("getY",                makeDelegate(&impl::getY));
    env->addCDelegate("getPosition",         makeDelegate(&impl::getPosition));
    env->addCDelegate("getPaddedPosition",   makeDelegate(&impl::getPaddedPosition));
    env->addCDelegate("getRelativeX",        makeDelegate(&impl::getRelativeX));
    env->addCDelegate("getRelativeY",        makeDelegate(&impl::getRelativeY));
    env->addCDelegate("getRelativePosition", makeDelegate(&impl::getRelativePosition));
    env->addCDelegate("getWidth",            makeDelegate(&impl::getWidth));
    env->addCDelegate("getHeight",           makeDelegate(&impl::getHeight));
    env->addCDelegate("getSize",             makeDelegate(&impl::getSize));
    env->addCDelegate("getPaddedSize",       makeDelegate(&impl::getPaddedSize));
    env->addCDelegate("getPadding",          makeDelegate(&impl::getPadding));
    env->addCDelegate("getPaddingLeft",      makeDelegate(&impl::getPaddingLeft));
    env->addCDelegate("getPaddingTop",       makeDelegate(&impl::getPaddingTop));
    env->addCDelegate("getPaddingRight",     makeDelegate(&impl::getPaddingRight));
    env->addCDelegate("getPaddingBottom",    makeDelegate(&impl::getPaddingBottom));
    env->addCDelegate("getClipping",         makeDelegate(&impl::getClipping));
    env->addCDelegate("getClippingLeft",     makeDelegate(&impl::getClippingLeft));
    env->addCDelegate("getClippingTop",      makeDelegate(&impl::getClippingTop));
    env->addCDelegate("getClippingRight",    makeDelegate(&impl::getClippingRight));
    env->addCDelegate("getClippingBottom",   makeDelegate(&impl::getClippingBottom));
    env->addCDelegate("getClipRect",         makeDelegate(&impl::getClipRect));
    env->addCDelegate("getZIndex",           makeDelegate(&impl::getZIndex));
    env->addCDelegate("getDock",             makeDelegate(&impl::getDock));
    env->addCDelegate("getDockState",        makeDelegate(&impl::getDockState));
    env->addCDelegate("getDockSize",         makeDelegate(&impl::getDockSize));
    env->addCDelegate("getName",             makeDelegate(&impl::getName));
    env->addCDelegate("getChildOffset",      makeDelegate(&impl::getChildOffset));
    env->addCDelegate("getChildOffsetX",     makeDelegate(&impl::getChildOffsetX));
    env->addCDelegate("getChildOffsetY",     makeDelegate(&impl::getChildOffsetY));
    env->addCDelegate("getFlags",            makeDelegate(&impl::getFlags));

    env->addCDelegate("isEnabled",                  makeDelegate(&impl::isEnabled));
    env->addCDelegate("isMouseIn",                  makeDelegate(&impl::isMouseIn));
    env->addCDelegate("isClicking",                 makeDelegate(&impl::isClicking));
    env->addCDelegate("ignoreOffset",               makeDelegate(&impl::ignoreOffset));
    env->addCDelegate("needsDimensionUpdate",       makeDelegate(&impl::needsDimensionUpdate));
    env->addCDelegate("needsZIndexReorder",         makeDelegate(&impl::needsZIndexReorder));
    env->addCDelegate("needsDockRecalculation",     makeDelegate(&impl::needsDockRecalculation));
    env->addCDelegate("needsClipRectRecalculation", makeDelegate(&impl::needsClipRectRecalculation));
    env->addCDelegate("needsDrawableRecalculation", makeDelegate(&impl::needsDrawableRecalculation));

    env->addCDelegate("getPositionType",     makeDelegate(&impl::getPositionType));
    env->addCDelegate("getRawPosition",      makeDelegate(&impl::getRawPosition));
    env->addCDelegate("getRawSize",          makeDelegate(&impl::getRawSize));
    env->addCDelegate("getRawLeft",          makeDelegate(&impl::getRawLeft));
    env->addCDelegate("getRawTop",           makeDelegate(&impl::getRawTop));
    env->addCDelegate("getRawRight",         makeDelegate(&impl::getRawRight));
    env->addCDelegate("getRawBottom",        makeDelegate(&impl::getRawBottom));
    env->addCDelegate("getRawMinSize",       makeDelegate(&impl::getRawMinSize));
    env->addCDelegate("getRawMaxSize",       makeDelegate(&impl::getRawMaxSize));
    env->addCDelegate("getRawDockSize",      makeDelegate(&impl::getRawDockSize));
    env->addCDelegate("getRawPadding",       makeDelegate(&impl::getRawPadding));
    env->addCDelegate("getRawPaddingLeft",   makeDelegate(&impl::getRawPaddingLeft));
    env->addCDelegate("getRawPaddingTop",    makeDelegate(&impl::getRawPaddingTop));
    env->addCDelegate("getRawPaddingRight",  makeDelegate(&impl::getRawPaddingRight));
    env->addCDelegate("getRawPaddingBottom", makeDelegate(&impl::getRawPaddingBottom));

    env->addCDelegate("addWidget",    makeDelegate(&impl::addWidget));
    env->addCDelegate("removeWidget", makeDelegate(&impl::removeWidget));

    env->addCDelegate("isInBounds", makeDelegate(&impl::isInBounds));

    env->addCDelegate("setX",                makeDelegate(&impl::setX));
    env->addCDelegate("setY",                makeDelegate(&impl::setY));
    env->addCDelegate("setPosition",         makeDelegate(&impl::setPosition));
    env->addCDelegate("setRelativeX",        makeDelegate(&impl::setRelativeX));
    env->addCDelegate("setRelativeY",        makeDelegate(&impl::setRelativeY));
    env->addCDelegate("setRelativePosition", makeDelegate(&impl::setRelativePosition));
    env->addCDelegate("setWidth",            makeDelegate(&impl::setWidth));
    env->addCDelegate("setHeight",           makeDelegate(&impl::setHeight));
    env->addCDelegate("setSize",             makeDelegate(&impl::setSize));
    env->addCDelegate("setPadding",          makeDelegate(&impl::setPadding));
    env->addCDelegate("setPaddingLeft",      makeDelegate(&impl::setPaddingLeft));
    env->addCDelegate("setPaddingTop",       makeDelegate(&impl::setPaddingTop));
    env->addCDelegate("setPaddingRight",     makeDelegate(&impl::setPaddingRight));
    env->addCDelegate("setPaddingBottom",    makeDelegate(&impl::setPaddingBottom));
    env->addCDelegate("setClipping",         makeDelegate(&impl::setClipping));
    env->addCDelegate("setClippingLeft",     makeDelegate(&impl::setClippingLeft));
    env->addCDelegate("setClippingTop",      makeDelegate(&impl::setClippingTop));
    env->addCDelegate("setClippingRight",    makeDelegate(&impl::setClippingRight));
    env->addCDelegate("setClippingBottom",   makeDelegate(&impl::setClippingBottom));
    env->addCDelegate("setZIndex",           makeDelegate(&impl::setZIndex));
    env->addCDelegate("setDock",             makeDelegate(&impl::setDock));
    env->addCDelegate("setDockState",        makeDelegate(&impl::setDockState));
    env->addCDelegate("setDockSize",         makeDelegate(&impl::setDockSize));
    env->addCDelegate("setName",             makeDelegate(&impl::setName));
    env->addCDelegate("setChildOffset",      makeDelegate(&impl::setChildOffset));
    env->addCDelegate("setChildOffsetX",     makeDelegate(&impl::setChildOffsetX));
    env->addCDelegate("setChildOffsetY",     makeDelegate(&impl::setChildOffsetY));
    env->addCDelegate("setFlags",            makeDelegate(&impl::setFlags));

    env->addCDelegate("setIgnoreOffset",               makeDelegate(&impl::ignoreOffset));
    env->addCDelegate("setNeedsDimensionUpdate",       makeDelegate(&impl::setNeedsDimensionUpdate));
    env->addCDelegate("setNeedsZIndexReorder",         makeDelegate(&impl::setNeedsZIndexReorder));
    env->addCDelegate("setNeedsDockRecalculation",     makeDelegate(&impl::setNeedsDockRecalculation));
    env->addCDelegate("setNeedsClipRectRecalculation", makeDelegate(&impl::setNeedsClipRectRecalculation));
    env->addCDelegate("setNeedsDrawableRecalculation", makeDelegate(&impl::setNeedsDrawableRecalculation));

    env->addCDelegate("setPositionType",     makeDelegate(&impl::setPositionType));
    env->addCDelegate("setRawPosition",      makeDelegate(&impl::setRawPosition));
    env->addCDelegate("setRawSize",          makeDelegate(&impl::setRawSize));
    env->addCDelegate("setLeft",             makeDelegate(&impl::setLeft));
    env->addCDelegate("setTop",              makeDelegate(&impl::setTop));
    env->addCDelegate("setRight",            makeDelegate(&impl::setRight));
    env->addCDelegate("setBottom",           makeDelegate(&impl::setBottom));
    env->addCDelegate("setRawLeft",          makeDelegate(&impl::setRawLeft));
    env->addCDelegate("setRawTop",           makeDelegate(&impl::setRawTop));
    env->addCDelegate("setRawRight",         makeDelegate(&impl::setRawRight));
    env->addCDelegate("setRawBottom",        makeDelegate(&impl::setRawBottom));
    env->addCDelegate("setMinSize",          makeDelegate(&impl::setMinSize));
    env->addCDelegate("setMaxSize",          makeDelegate(&impl::setMaxSize));
    env->addCDelegate("setRawMinSize",       makeDelegate(&impl::setRawMinSize));
    env->addCDelegate("setRawMaxSize",       makeDelegate(&impl::setRawMaxSize));
    env->addCDelegate("setRawDockSize",      makeDelegate(&impl::setRawDockSize));
    env->addCDelegate("setRawPadding",       makeDelegate(&impl::setRawPadding));
    env->addCDelegate("setRawPaddingLeft",   makeDelegate(&impl::setRawPaddingLeft));
    env->addCDelegate("setRawPaddingTop",    makeDelegate(&impl::setRawPaddingTop));
    env->addCDelegate("setRawPaddingRight",  makeDelegate(&impl::setRawPaddingRight));
    env->addCDelegate("setRawPaddingBottom", makeDelegate(&impl::setRawPaddingBottom));

    // TODO(Matthew): Need to give ownership of these delegates to someone in order to not end up leaking them.
    env->addCDelegate("onMouseClick", makeFunctor([=](Widget* widget, nString name) {
        auto del = env->template getScriptDelegate<void, Sender, const MouseButtonEvent&>(name);

        widget->MouseClick.add(*del, true);

        del->neuter();
        delete del;
    }));
    env->addCDelegate("onMouseDown",  makeFunctor([=](Widget* widget, nString name) {
        auto del = env->template getScriptDelegate<void, Sender, const MouseButtonEvent&>(name);

        widget->MouseDown.add(*del, true);

        del->neuter();
        delete del;
    }));
    env->addCDelegate("onMouseUp",    makeFunctor([=](Widget* widget, nString name) {
        auto del = env->template getScriptDelegate<void, Sender, const MouseButtonEvent&>(name);

        widget->MouseUp.add(*del, true);

        del->neuter();
        delete del;
    }));
    env->addCDelegate("onMouseEnter", makeFunctor([=](Widget* widget, nString name) {
        auto del = env->template getScriptDelegate<void, Sender, const MouseMotionEvent&>(name);

        widget->MouseEnter.add(*del, true);

        del->neuter();
        delete del;
    }));
    env->addCDelegate("onMouseLeave", makeFunctor([=](Widget* widget, nString name) {
        auto del = env->template getScriptDelegate<void, Sender, const MouseMotionEvent&>(name);

        widget->MouseLeave.add(*del, true);

        del->neuter();
        delete del;
    }));
    env->addCDelegate("onMouseMove",  makeFunctor([=](Widget* widget, nString name) {
        auto del = env->template getScriptDelegate<void, Sender, const MouseMotionEvent&>(name);

        widget->MouseMove.add(*del, true);

        del->neuter();
        delete del;
    }));
    env->setNamespaces();
}

template <typename ScriptEnvironmentImpl>
void vui::WidgetScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", "ClippingState");
    env->addValue("VISIBLE", ClippingState::VISIBLE);
    env->addValue("HIDDEN",  ClippingState::HIDDEN);
    env->addValue("INHERIT", ClippingState::INHERIT);
    env->setNamespaces();

    env->setNamespaces("UI", "DockState");
    env->addValue("NONE",   DockState::NONE);
    env->addValue("LEFT",   DockState::LEFT);
    env->addValue("RIGHT",  DockState::RIGHT);
    env->addValue("BOTTOM", DockState::BOTTOM);
    env->addValue("TOP",    DockState::TOP);
    env->addValue("FILL",   DockState::FILL);
    env->setNamespaces();

    env->setNamespaces("UI", "PositionType");
    env->addValue("STATIC_TO_WINDOW",     PositionType::STATIC_TO_WINDOW);
    env->addValue("STATIC_TO_VIEWPORT",   PositionType::STATIC_TO_VIEWPORT);
    env->addValue("STATIC_TO_PARENT",     PositionType::STATIC_TO_PARENT);
    env->addValue("RELATIVE_TO_WINDOW",   PositionType::RELATIVE_TO_WINDOW);
    env->addValue("RELATIVE_TO_VIEWPORT", PositionType::RELATIVE_TO_VIEWPORT);
    env->addValue("RELATIVE_TO_PARENT",   PositionType::RELATIVE_TO_PARENT);
    env->setNamespaces();

    env->setNamespaces("UI", "DimensionType");
    env->addValue("PIXEL",                      DimensionType::PIXEL);
    env->addValue("WIDTH_PERCENTAGE",           DimensionType::WIDTH_PERCENTAGE);
    env->addValue("HEIGHT_PERCENTAGE",          DimensionType::HEIGHT_PERCENTAGE);
    env->addValue("MIN_PERCENTAGE",             DimensionType::MIN_PERCENTAGE);
    env->addValue("MAX_PERCENTAGE",             DimensionType::MAX_PERCENTAGE);
    env->addValue("PARENT_WIDTH_PERCENTAGE",    DimensionType::PARENT_WIDTH_PERCENTAGE);
    env->addValue("PARENT_HEIGHT_PERCENTAGE",   DimensionType::PARENT_HEIGHT_PERCENTAGE);
    env->addValue("PARENT_MIN_PERCENTAGE",      DimensionType::PARENT_MIN_PERCENTAGE);
    env->addValue("PARENT_MAX_PERCENTAGE",      DimensionType::PARENT_MAX_PERCENTAGE);
    env->addValue("VIEWPORT_WIDTH_PERCENTAGE",  DimensionType::VIEWPORT_WIDTH_PERCENTAGE);
    env->addValue("VIEWPORT_HEIGHT_PERCENTAGE", DimensionType::VIEWPORT_HEIGHT_PERCENTAGE);
    env->addValue("VIEWPORT_MIN_PERCENTAGE",    DimensionType::VIEWPORT_MIN_PERCENTAGE);
    env->addValue("VIEWPORT_MAX_PERCENTAGE",    DimensionType::VIEWPORT_MAX_PERCENTAGE);
    env->addValue("WINDOW_WIDTH_PERCENTAGE",    DimensionType::WINDOW_WIDTH_PERCENTAGE);
    env->addValue("WINDOW_HEIGHT_PERCENTAGE",   DimensionType::WINDOW_HEIGHT_PERCENTAGE);
    env->addValue("WINDOW_MIN_PERCENTAGE",      DimensionType::WINDOW_MIN_PERCENTAGE);
    env->addValue("WINDOW_MAX_PERCENTAGE",      DimensionType::WINDOW_MAX_PERCENTAGE);
    env->setNamespaces();
}

#endif // !Vorb_WidgetScriptFuncs_h__
