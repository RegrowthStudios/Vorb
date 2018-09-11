#include "Vorb/stdafx.h"
#include "Vorb/ui/IWidget.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/utils.h"

vui::IWidget::IWidget(UIRenderer* renderer /*= nullptr*/, const GameWindow* window /*= nullptr*/) :
    MouseClick(this),
    MouseDown(this),
    MouseUp(this),
    MouseEnter(this),
    MouseLeave(this),
    MouseMove(this),
    m_font(nullptr),
    m_window(window),
    m_canvas(this),
    m_parent(nullptr),
    m_widgets(IWidgets()),
    m_clipRect(f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX)),
    // m_dockSizes(f32v4(0.0f)),
    m_position(f32v2(0.0f)),
    m_size(f32v2(0.0f)),
    m_name(""),
    m_isClicking(false),
    m_isEnabled(false),
    m_isMouseIn(false) {
    // m_style = {};
    // As the widget has just been made, it is its own canvas - thus should be subscribed for resize events.
    vui::InputDispatcher::window.onResize += makeDelegate(*this, &IWidget::onResize);
    enable();
    if (renderer) {
        setRenderer(renderer);
    } else {
        m_renderer = renderer;
    }
}

vui::IWidget::~IWidget() {
    // Empty
}

void vui::IWidget::dispose() {
    for (auto& w : m_widgets) {
        w->dispose();
    }
    IWidgets().swap(m_widgets);
    // for (int i = 0; i < 5; i++) IWidgets().swap(m_dockedWidgets[i]);

    disable();
    
}

bool vui::IWidget::addWidget(IWidget* child) {
    m_widgets.push_back(child);
    child->m_parent = this;
    child->updateDimensions();
    return true; // TODO(Ben): Is this needed?
}

bool vui::IWidget::removeWidget(IWidget* child) {
    for (auto it = m_widgets.begin(); it != m_widgets.end(); it++) {
        if (*it == child) {
            // if (removeChildFromDock(child)) recalculateDockedWidgets();
            m_widgets.erase(it);
            return true;
        }
    }
    return false;
}

void vui::IWidget::enable() {
    if (!m_isEnabled) {
        m_isEnabled = true;
        vui::InputDispatcher::mouse.onButtonDown += makeDelegate(*this, &IWidget::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp += makeDelegate(*this, &IWidget::onMouseUp);
        vui::InputDispatcher::mouse.onMotion += makeDelegate(*this, &IWidget::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost += makeDelegate(*this, &IWidget::onMouseFocusLost);
    }
    // Enable all children
    for (auto& w : m_widgets) w->enable();
}

void vui::IWidget::disable() {
    if (m_isEnabled) {
        m_isEnabled = false;
        vui::InputDispatcher::mouse.onButtonDown -= makeDelegate(*this, &IWidget::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp -= makeDelegate(*this, &IWidget::onMouseUp);
        vui::InputDispatcher::mouse.onMotion -= makeDelegate(*this, &IWidget::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost -= makeDelegate(*this, &IWidget::onMouseFocusLost);
    }
    m_isClicking = false;
    // Disable all children
    for (auto& w : m_widgets) w->disable();
}

bool vui::IWidget::isInBounds(f32 x, f32 y) const {
    return (x >= m_position.x && x < m_position.x + m_size.x &&
            y >= m_position.y && y < m_position.y + m_size.y);
}

void vui::IWidget::setParent(IWidget* parent, bool keepOwnRenderer /*= false*/) {
    // Remove this widget from any previous parent it may have had.
    if (m_parent) m_parent->removeWidget(this);
    // Are we are setting a new parent?
    if (parent) {
        // Add widget to the new parent (which will set the m_parent field).
        parent->addWidget(this);

        if (m_canvas == this) {
            // As the previous canvas widget, we should unsubscribe it.
            vui::InputDispatcher::window.onResize -= makeDelegate(*this, &IWidget::onResize);
        }

        // Set the new canvas widget of this widget.
        m_canvas = parent->getCanvas();
        // Set the renderer for this widget to that of the new parent if we are not keeping the old renderer.
        if (!keepOwnRenderer &&
            getRenderer() != parent->getRenderer()) setRenderer(parent->getRenderer());
        // Propagate the new canvas widget to children.
        updateChildCanvases();
    } else {
        // Set the canvas to this widget.
        m_canvas = this;
        // As canvas, this widget is now responsible for handling resizing of itself and children.
        vui::InputDispatcher::window.onResize += makeDelegate(*this, &IWidget::onResize);
        // Update children - this widget is the new canvas widget.
        updateChildCanvases();
    }

    // Parent (and maybe canvas changing) means we should reevaluate dimensions of this widget and its children.
    updateDimensions();

    updateChildDimensions();
}

// void vui::IWidget::setChildDock(Widget* widget, DockStyle dockStyle) {
//     removeChildFromDock(widget);
//     // Add to new dock
//     widget->m_dock = dockStyle;
//     int dockIndex = (int)dockStyle - 1;
//     if (dockIndex != -1) {
//         m_dockedWidgets[dockIndex].push_back(widget);
//     }
//     recalculateDockedWidgets();
// }

// void vui::IWidget::setDestRect(const f32v4& destRect) {
//     m_relativePosition.x = destRect.x;
//     m_relativePosition.y = destRect.y;
//     m_dimensions.x = destRect.z;
//     m_dimensions.y = destRect.w;
//     updatePosition();
// }

// bool vui::IWidget::removeChildFromDock(Widget* widget) {
//     int dockIndex = (int)widget->m_dock - 1;
//     // Remove it from its current dock
//     if (dockIndex != -1) {
//         for (auto w = m_dockedWidgets[dockIndex].begin(); w != m_dockedWidgets[dockIndex].end(); w++) {
//             if (*w == widget) {
//                 m_dockedWidgets[dockIndex].erase(w);
//                 // Subtract size
//                 switch (widget->m_dock) {
//                     case DockStyle::LEFT:
//                     case DockStyle::RIGHT:
//                         m_dockSizes[dockIndex] -= widget->getRawWidth();
//                         break;
//                     case DockStyle::BOTTOM:
//                     case DockStyle::TOP:
//                         m_dockSizes[dockIndex] -= widget->getRawHeight();
//                         break;
//                     default:
//                         break;
//                 }
//                 break;
//             }
//         }
//         widget->m_dock = DockStyle::NONE;
//         return true;
//     }
//     return false;
// }

// void vui::IWidget::recalculateDockedWidgets() {
//     // Bottom
//     m_dockSizes[2] = 0.0f;
//     for (auto& w : m_dockedWidgets[2]) {
//         w->setWidth(m_dimensions.x);
//         m_dockSizes[2] += w->getHeight();
//         w->setPosition(f32v2(0.0f, m_dimensions.y - m_dockSizes[2]));
//     }
//     // Top
//     m_dockSizes[3] = 0.0f;
//     for (auto& w : m_dockedWidgets[3]) {
//         w->setWidth(m_dimensions.x);
//         w->setPosition(f32v2(0.0f, m_dockSizes[3]));
//         m_dockSizes[3] += w->getHeight();
//     }
//     // Left
//     m_dockSizes[0] = 0.0f;
//     for (auto& w : m_dockedWidgets[0]) {
//         w->setHeight(m_dimensions.y - m_dockSizes[2] - m_dockSizes[3]);
//         w->setPosition(f32v2(m_dockSizes[0], m_dockSizes[3]));
//         m_dockSizes[0] += w->getWidth();
//     }
//     // Right
//     m_dockSizes[1] = 0.0f;
//     for (auto& w : m_dockedWidgets[1]) {
//         w->setHeight(m_dimensions.y - m_dockSizes[2] - m_dockSizes[3]);
//         m_dockSizes[1] += w->getWidth();
//         w->setPosition(f32v2(m_dimensions.x - m_dockSizes[1], m_dockSizes[3]));
//     }
//     // Fill
//     for (auto& w : m_dockedWidgets[4]) {
//         w->setHeight(m_dimensions.y - m_dockSizes[2] - m_dockSizes[3]);
//         w->setWidth(m_dimensions.x - m_dockSizes[0] - m_dockSizes[1]);
//         w->setPosition(f32v2(m_dockSizes[0], m_dockSizes[3]));
//     }
// }

// void vui::IWidget::computeClipRect(const f32v4& parentClipRect /*= f32v4(-FLT_MAX / 2.0f, -FLT_MAX / 2.0f, FLT_MAX, FLT_MAX)*/) {
//     if (m_isClippingEnabled) {
//         f32v2 pos = m_position;
//         f32v2 dims = m_size;
//         computeClipping(parentClipRect, pos, dims);
//         if (dims.x < 0) dims.x = 0;
//         if (dims.y < 0) dims.y = 0;
//         m_clipRect = f32v4(pos.x, pos.y, dims.x, dims.y);
//     } else {
//         m_clipRect = parentClipRect;
//     }
//     computeChildClipRects();
// }

// void vui::IWidget::computeChildClipRects() {
//     for (auto& w : m_widgets) {
//         w->computeClipRect(m_clipRect);
//     }
// }

void vui::IWidget::updateChildDimensions() {
    for (IWidget* widget : m_widgets) {
        widget->updateDimensions();
    }
}

void vui::IWidget::updateChildCanvases() {
    for (IWidget* widget : m_widgets) {
        widget->m_canvas = m_canvas;
        widget->updateChildCanvases();
    }
}

void vui::IWidget::addDrawables() {
    for (auto& child : m_widgets) {
        child->addDrawables();
    }
}

void vui::IWidget::removeDrawables() {
    if (m_renderer) m_renderer->remove(this);

    for (auto& child : m_widgets) {
        child->removeDrawables();
    }
}

void vui::IWidget::onMouseDown(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseDown(e);
        m_isClicking = true;
    }
}

void vui::IWidget::onMouseUp(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) MouseClick(e);
    }
    m_isClicking = false;
}

void vui::IWidget::onMouseMove(Sender s VORB_UNUSED, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_isMouseIn) {
            m_isMouseIn = true;
            MouseEnter(e);
        }
        MouseMove(e);
    } else if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseLeave(e);
    }
}

void vui::IWidget::onMouseFocusLost(Sender s VORB_UNUSED, const MouseEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
    }
}

void vui::IWidget::onResize(Sender s VORB_MAYBE_UNUSED, const WindowResizeEvent& e VORB_MAYBE_UNUSED) {
    updateDimensions();
}