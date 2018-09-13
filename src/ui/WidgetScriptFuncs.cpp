#include "Vorb/stdafx.h"
#include "Vorb/ui/WidgetScriptFuncs.h"
#include "Vorb/script/Environment.h"
#include "Vorb/ui/MouseInputDispatcher.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &WidgetScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &WidgetScriptFuncs::name));

void vui::WidgetScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    m_env = env;
    env->setNamespaces(nSpace);

    { // Register all functions
        // Getters
        // REGISTER_RDEL(env, getFixedHeight);
        // REGISTER_RDEL(env, getFixedWidth);
        // REGISTER_RDEL(env, getSelectable);
        REGISTER_RDEL(env, isMouseIn);
        REGISTER_RDEL(env, getAnchor);
        REGISTER_RDEL(env, getStyle);
        // REGISTER_RDEL(env, getDock);
        REGISTER_RDEL(env, getNumWidgets);
        REGISTER_RDEL(env, isEnabled);
        // REGISTER_RDEL(env, getClippingEnabled);
        REGISTER_RDEL(env, getHeight);
        REGISTER_RDEL(env, getWidth);
        REGISTER_RDEL(env, getX);
        REGISTER_RDEL(env, getY);
        // REGISTER_RDEL(env, getDimensions);
        REGISTER_RDEL(env, getPosition);
        // REGISTER_RDEL(env, getRelativePosition);
        REGISTER_RDEL(env, getName);
        REGISTER_RDEL(env, getDestRect);
        REGISTER_RDEL(env, getParent);
        // REGISTER_RDEL(env, getMinSize);
        // REGISTER_RDEL(env, getMaxSize);
        // REGISTER_RDEL(env, getPositionPercentage);
        // REGISTER_RDEL(env, getDimensionsPercentage);
        // REGISTER_RDEL(env, getWidgetAlign);
        // Setters
        REGISTER_DEL(env, setAnchor);
        // REGISTER_DEL(env, setDestRect);
        // REGISTER_DEL(env, setDimensions);
        // REGISTER_DEL(env, setDock);
        // REGISTER_DEL(env, setFixedHeight);
        // REGISTER_DEL(env, setFixedWidth);
        // REGISTER_DEL(env, setHeight);
        // REGISTER_DEL(env, setPosition);
        // REGISTER_DEL(env, setSelectable);
        REGISTER_DEL(env, setStyle);
        // REGISTER_DEL(env, setWidth);
        // REGISTER_DEL(env, setX);
        // REGISTER_DEL(env, setY);
        REGISTER_DEL(env, setName);
        REGISTER_DEL(env, setParent);
        // REGISTER_DEL(env, setMinSize);
        // REGISTER_DEL(env, setMaxSize);
        // REGISTER_DEL(env, setPositionPercentage);
        // REGISTER_DEL(env, setDimensionsPercentage);
        // REGISTER_DEL(env, setXPercentage);
        // REGISTER_DEL(env, setYPercentage);
        // REGISTER_DEL(env, setWidthPercentage);
        // REGISTER_DEL(env, setHeightPercentage);
        // REGISTER_DEL(env, setWidgetAlign);
        // REGISTER_DEL(env, setClippingEnabled);
        // Misc
        REGISTER_DEL(env, dispose);
        REGISTER_DEL(env, enable);
        REGISTER_DEL(env, disable);
        REGISTER_RDEL(env, addCallback);
        REGISTER_RDEL(env, removeCallback);
    }

    { // Register EventType enum
        env->setNamespaces("EventType");
        env->addValue("NONE", vui::EventType::NONE);
        env->addValue("MOUSE_CLICK", vui::EventType::MOUSE_CLICK);
        env->addValue("MOUSE_DOWN", vui::EventType::MOUSE_DOWN);
        env->addValue("MOUSE_UP", vui::EventType::MOUSE_UP);
        env->addValue("MOUSE_ENTER", vui::EventType::MOUSE_ENTER);
        env->addValue("MOUSE_LEAVE", vui::EventType::MOUSE_LEAVE);
        env->addValue("MOUSE_MOVE", vui::EventType::MOUSE_MOVE);
        env->addValue("VALUE_CHANGE", vui::EventType::VALUE_CHANGE);
    }

    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

void vui::WidgetScriptFuncs::registerWidget(Widget* w) {
    w->MouseClick += makeDelegate(*this, &WidgetScriptFuncs::onMouseClick);
    w->MouseDown += makeDelegate(*this, &WidgetScriptFuncs::onMouseDown);
    w->MouseUp += makeDelegate(*this, &WidgetScriptFuncs::onMouseUp);
    w->MouseMove += makeDelegate(*this, &WidgetScriptFuncs::onMouseMove);
    w->MouseEnter += makeDelegate(*this, &WidgetScriptFuncs::onMouseEnter);
    w->MouseLeave += makeDelegate(*this, &WidgetScriptFuncs::onMouseLeave);
}

void vui::WidgetScriptFuncs::unregisterWidget(Widget* w) {
    w->MouseClick -= makeDelegate(*this, &WidgetScriptFuncs::onMouseClick);
    w->MouseDown -= makeDelegate(*this, &WidgetScriptFuncs::onMouseDown);
    w->MouseUp -= makeDelegate(*this, &WidgetScriptFuncs::onMouseUp);
    w->MouseMove -= makeDelegate(*this, &WidgetScriptFuncs::onMouseMove);
    w->MouseEnter -= makeDelegate(*this, &WidgetScriptFuncs::onMouseEnter);
    w->MouseLeave -= makeDelegate(*this, &WidgetScriptFuncs::onMouseLeave);
}

void vui::WidgetScriptFuncs::dispose(Widget* w) {
    w->dispose();
}

void vui::WidgetScriptFuncs::enable(Widget* w) {
    w->enable();
}

void vui::WidgetScriptFuncs::disable(Widget* w) {
    w->disable();
}

bool vui::WidgetScriptFuncs::addCallback(Widget* w, EventType eventType, nString funcName) {
    const vscript::Function& f = (*m_env)[funcName];
    if (f.isNil()) return false;
    switch (eventType) {
        case EventType::MOUSE_CLICK:
            w->m_mouseClickFuncs.push_back(f); break;
        case EventType::MOUSE_DOWN:
            w->m_mouseDownFuncs.push_back(f); break;
        case EventType::MOUSE_ENTER:
            w->m_mouseEnterFuncs.push_back(f); break;
        case EventType::MOUSE_LEAVE:
            w->m_mouseLeaveFuncs.push_back(f); break;
        case EventType::MOUSE_MOVE:
            w->m_mouseMoveFuncs.push_back(f); break;
        case EventType::MOUSE_UP:
            w->m_mouseUpFuncs.push_back(f); break;
        default:
            return false;
    }
    return true;
}

bool vui::WidgetScriptFuncs::removeCallback(Widget* w, EventType eventType, nString funcName) {
    const vscript::Function& f = (*m_env)[funcName];
    if (f.isNil()) return false;
    switch (eventType) {
        case EventType::MOUSE_CLICK:
            w->m_mouseClickFuncs.erase(std::find(w->m_mouseClickFuncs.begin(), w->m_mouseClickFuncs.end(), f));
            break;
        case EventType::MOUSE_DOWN:
            w->m_mouseDownFuncs.erase(std::find(w->m_mouseDownFuncs.begin(), w->m_mouseDownFuncs.end(), f));
            break;
        case EventType::MOUSE_ENTER:
            w->m_mouseEnterFuncs.erase(std::find(w->m_mouseEnterFuncs.begin(), w->m_mouseEnterFuncs.end(), f));
            break;
        case EventType::MOUSE_LEAVE:
            w->m_mouseLeaveFuncs.erase(std::find(w->m_mouseLeaveFuncs.begin(), w->m_mouseLeaveFuncs.end(), f));
            break;
        case EventType::MOUSE_MOVE:
            w->m_mouseMoveFuncs.erase(std::find(w->m_mouseMoveFuncs.begin(), w->m_mouseMoveFuncs.end(), f));
            break;
        case EventType::MOUSE_UP:
            w->m_mouseUpFuncs.erase(std::find(w->m_mouseUpFuncs.begin(), w->m_mouseUpFuncs.end(), f));
            break;
        default:
            return false;
    }
    return true;
}

// bool vui::WidgetScriptFuncs::getFixedHeight(Widget* w) const {
//     return w->getFixedHeight();
// }

// bool vui::WidgetScriptFuncs::getFixedWidth(Widget* w) const {
//     return w->getFixedWidth();
// }

// bool vui::WidgetScriptFuncs::getSelectable(Widget* w) const {
//     return w->getSelectable();
// }

bool vui::WidgetScriptFuncs::isMouseIn(Widget* w) const {
    return w->isMouseIn();
}

i32 vui::WidgetScriptFuncs::getAnchor(Widget* w VORB_UNUSED) const {
    return 0; //TODO(Ben): Implement
}

i32 vui::WidgetScriptFuncs::getStyle(Widget* w VORB_UNUSED) const {
    return 0; //TODO(Ben): Implement
}

// vui::Dock vui::WidgetScriptFuncs::getDock(Widget* w) const {
//     return w->getDock();
// }

i32 vui::WidgetScriptFuncs::getNumWidgets(Widget* w) const {
    return (i32)w->getWidgets().size();
}

bool vui::WidgetScriptFuncs::isEnabled(Widget* w) const {
    return w->isEnabled();
}

// bool vui::WidgetScriptFuncs::getClippingEnabled(Widget* w) const {
//     return w->getClippingEnabled();
// }

f32 vui::WidgetScriptFuncs::getHeight(Widget* w) const {
    return w->getHeight();
}

f32 vui::WidgetScriptFuncs::getWidth(Widget* w) const {
    return w->getWidth();
}

f32 vui::WidgetScriptFuncs::getX(Widget* w) const {
    return w->getX();
}

f32 vui::WidgetScriptFuncs::getY(Widget* w) const {
    return w->getY();
}

// f32v2 vui::WidgetScriptFuncs::getDimensions(Widget* w) const {
//     return w->getDimensions();
// }

f32v2 vui::WidgetScriptFuncs::getPosition(Widget* w) const {
    return w->getPosition();
}

// f32v2 vui::WidgetScriptFuncs::getRelativePosition(Widget* w) const {
//     return w->getRelativePosition();
// }

nString vui::WidgetScriptFuncs::getName(Widget* w) const {
    return w->getName();
}

// f32v4 vui::WidgetScriptFuncs::getDestRect(Widget* w) const {
//     return w->getDestRect();
// }

vui::IWidget* vui::WidgetScriptFuncs::getParent(Widget* w) const {
    return w->getParent();
}

// f32v2 vui::WidgetScriptFuncs::getMinSize(Widget* w) const {
//     return w->getMinSize();
// }

// f32v2 vui::WidgetScriptFuncs::getMaxSize(Widget* w) const {
//     return w->getMaxSize();
// }

// f32v2 vui::WidgetScriptFuncs::getPositionPercentage(Widget* w) const {
//     return w->getPositionPercentage();
// }

// f32v2 vui::WidgetScriptFuncs::getDimensionsPercentage(Widget* w) const {
//     return w->getDimensionsPercentage();
// }

// vui::WidgetAlign vui::WidgetScriptFuncs::getWidgetAlign(Widget* w) const {
//     return w->getWidgetAlign();
// }

void vui::WidgetScriptFuncs::setAnchor(Widget* w VORB_UNUSED, int anchor VORB_UNUSED) const {
    //TODO(Ben): Implement
}

// void vui::WidgetScriptFuncs::setDestRect(Widget* w, f32v4 destRect) const {
//     w->setDestRect(destRect);
// }

// void vui::WidgetScriptFuncs::setDimensions(Widget* w, f32v2 dims) const {
//     w->setDimensions(dims);
// }

// void vui::WidgetScriptFuncs::setDock(Widget* w, Dock dock) const {
//     w->setDock(dock);
// }

// void vui::WidgetScriptFuncs::setFixedHeight(Widget* w, bool fixedHeight) const {
//     w->setFixedHeight(fixedHeight);
// }

// void vui::WidgetScriptFuncs::setFixedWidth(Widget* w, bool fixedWidth) const {
//     w->setFixedWidth(fixedWidth);
// }

// void vui::WidgetScriptFuncs::setHeight(Widget* w, f32 height) const {
//     w->setHeight(height);
// }

// void vui::WidgetScriptFuncs::setPosition(Widget* w, f32v2 pos) const {
//     w->setPosition(pos);
// }

// void vui::WidgetScriptFuncs::setSelectable(Widget* w, bool selectable) const {
//     w->setSelectable(selectable);
// }

void vui::WidgetScriptFuncs::setStyle(Widget* w VORB_UNUSED, int style VORB_UNUSED) const {
    //TODO(Ben): Implement
}

// void vui::WidgetScriptFuncs::setWidth(Widget* w, f32 width) const {
//     w->setWidth(width);
// }

// void vui::WidgetScriptFuncs::setX(Widget* w, f32 x) const {
//     w->setX(x);
// }

// void vui::WidgetScriptFuncs::setY(Widget* w, f32 y) const {
//     w->setY(y);
// }

void vui::WidgetScriptFuncs::setName(Widget* w, nString name) const {
    w->setName(name);
}

void vui::WidgetScriptFuncs::setParent(Widget* w, IWidget* parent) const {
    w->setParent(parent);
}

// void vui::WidgetScriptFuncs::setMinSize(Widget* w, f32v2 minSize) const {
//     w->setMinSize(minSize);
// }

// void vui::WidgetScriptFuncs::setMaxSize(Widget* w, f32v2 maxSize) const {
//     w->setMaxSize(maxSize);
// }

// void vui::WidgetScriptFuncs::setPositionPercentage(Widget* w, f32v2 positionPercentage) const {
//     w->setPositionPercentage(positionPercentage);
// }

// void vui::WidgetScriptFuncs::setDimensionsPercentage(Widget* w, f32v2 dimensionsPercentage) const {
//     w->setDimensionsPercentage(dimensionsPercentage);
// }

// void vui::WidgetScriptFuncs::setXPercentage(Widget* w, f32 xPercentage) const {
//     w->setXPercentage(xPercentage);
// }

// void vui::WidgetScriptFuncs::setYPercentage(Widget* w, f32 yPercentage) const {
//     w->setYPercentage(yPercentage);
// }

// void vui::WidgetScriptFuncs::setWidthPercentage(Widget* w, f32 widthPercentage) const {
//     w->setWidthPercentage(widthPercentage);
// }

// void vui::WidgetScriptFuncs::setHeightPercentage(Widget* w, f32 heightPercentage) const {
//     w->setHeightPercentage(heightPercentage);
// }

// void vui::WidgetScriptFuncs::setWidgetAlign(Widget* w, WidgetAlign widgetAlign) const {
//     w->setWidgetAlign(widgetAlign);
// }

// void vui::WidgetScriptFuncs::setClippingEnabled(Widget* w, bool clippingEnabled) const {
//     w->setClippingEnabled(clippingEnabled);
// }

void vui::WidgetScriptFuncs::onMouseClick(Sender s, const MouseButtonEvent& e) {
    Widget* w = (Widget*)s;
    size_t sz = w->m_mouseClickFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_mouseClickFuncs.size(); i++) {
        w->m_mouseClickFuncs[i](e.button, e.x, e.y);
    }
}

void vui::WidgetScriptFuncs::onMouseDown(Sender s, const MouseButtonEvent& e) {
    Widget* w = (Widget*)s;
    size_t sz = w->m_mouseDownFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_mouseDownFuncs.size(); i++) {
        w->m_mouseDownFuncs[i](e.button, e.x, e.y);
    }
}

void vui::WidgetScriptFuncs::onMouseUp(Sender s, const MouseButtonEvent& e) {
    Widget* w = (Widget*)s;
    size_t sz = w->m_mouseUpFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_mouseUpFuncs.size(); i++) {
        w->m_mouseUpFuncs[i](e.button, e.x, e.y);
    }
}

void vui::WidgetScriptFuncs::onMouseEnter(Sender s, const MouseMotionEvent& e) {
    Widget* w = (Widget*)s;
    size_t sz = w->m_mouseEnterFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_mouseEnterFuncs.size(); i++) {
        w->m_mouseEnterFuncs[i](e.x, e.y);
    }
}

void vui::WidgetScriptFuncs::onMouseLeave(Sender s, const MouseMotionEvent& e) {
    Widget* w = (Widget*)s;
    size_t sz = w->m_mouseLeaveFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_mouseLeaveFuncs.size(); i++) {
        w->m_mouseLeaveFuncs[i](e.x, e.y);
    }
}

void vui::WidgetScriptFuncs::onMouseMove(Sender s, const MouseMotionEvent& e) {
    Widget* w = (Widget*)s;
    size_t sz = w->m_mouseMoveFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_mouseMoveFuncs.size(); i++) {
        w->m_mouseMoveFuncs[i](e.x, e.y);
    }
}