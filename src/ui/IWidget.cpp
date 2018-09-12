#include "Vorb/stdafx.h"
#include "Vorb/ui/IWidget.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/utils.h"

vui::IWidget::IWidget(UIRenderer* renderer, const GameWindow* window /*= nullptr*/) :
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
    // m_dockSizes(f32v4(0.0f)),
    m_position(f32v2(0.0f)),
    m_size(f32v2(0.0f)),
    m_clipping(DEFAULT_CLIPPING),
    m_name(""),
    m_isClicking(false),
    m_isEnabled(false),
    m_isMouseIn(false),
    m_needsClipRectRecalculation(false),
    m_needsDrawableRefresh(false),
    m_needsDimensionUpdate(false) {
    // m_style = {};
    // As the widget has just been made, it is its own canvas - thus should be subscribed for resize events.
    resetClipRect();
    vui::InputDispatcher::window.onResize += makeDelegate(*this, &IWidget::onResize);
    enable();
    setRenderer(renderer);
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

    removeDrawables();

    disable();
}

void vui::IWidget::update(f32 dt) {
    if (m_needsDimensionUpdate) {
        m_needsDimensionUpdate = false;
        updateDimensions();
    }

    if (m_needsClipRectRecalculation) {
        m_needsClipRectRecalculation = false;
        calculateClipRect();
    }

    if (m_needsDrawableRefresh) {
        m_needsDrawableRefresh = false;
        refreshDrawables();
    }
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

bool vui::IWidget::addWidget(IWidget* child) {
    m_widgets.push_back(child);
    child->m_parent = this;
    child->m_needsDimensionUpdate = true;
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

bool vui::IWidget::isInBounds(f32 x, f32 y) const {
    return (x >= m_position.x && x < m_position.x + m_size.x &&
            y >= m_position.y && y < m_position.y + m_size.y);
}

void vui::IWidget::setParent(IWidget* parent) {
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
        
        // Propagate the new canvas widget to children.
        updateChildCanvases();
    } else {
        // Set the canvas to this widget.
        m_canvas = this;
        // Reset clip rect to recalculate correctly.
        resetClipRect();
        // As canvas, this widget is now responsible for handling resizing of itself and children.
        vui::InputDispatcher::window.onResize += makeDelegate(*this, &IWidget::onResize);
        // Update children - this widget is the new canvas widget.
        updateChildCanvases();
    }

    // Parent (and maybe canvas changing) means we should reevaluate dimensions of this widget and its children.
    m_needsDimensionUpdate       = true;
    m_needsClipRectRecalculation = true;
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

void vui::IWidget::calculateClipRect() {
    // TODO(Matthew): Revisit inherit.
    clipRect = m_parent->getClipRect();

    ClippingState left = getClippingLeft();
    if (m_position.x < clipRect.x) {
        m_clipRect.x = clipRect.x;
    } else if (left == ClippingState::HIDDEN) {
        m_clipRect.x = m_position.x;
    } else if (left == ClippingState::VISIBLE) {
        m_clipRect.x = m_parent ? m_parent->getClipRect().left
                                : m_canvas->getClipRect().left;
    } else {
        // Shouldn't get here as getClipping{Left|Top|Right|Bottom} should only return HIDDEN or VISIBLE.
        assert(false);
    }

    ClippingState top = getClippingTop();
    if (m_position.y < clipRect.y) {
        m_clipRect.y = clipRect.y;
    } else if (top == ClippingState::HIDDEN) {
        m_clipRect.y = m_position.y;
    } else if (top == ClippingState::VISIBLE) {
        m_clipRect.y = m_parent ? m_parent->getClipRect().top
                                : m_canvas->getClipRect().top;
    } else {
        // Shouldn't get here as getClipping{Left|Top|Right|Bottom} should only return HIDDEN or VISIBLE.
        assert(false);
    }

    ClippingState right = getClippingRight();
    if (m_position.x + m_size.x > clipRect.x + clipRect.z) {
        m_clipRect.z = clipRect.x + clipRect.z - m_clipRect.x;
    } else if (right == ClippingState::HIDDEN) {
        m_clipRect.z = m_position.x + m_size.x - m_clipRect.x;
    } else if (right == ClippingState::VISIBLE) {
        m_clipRect.z = m_parent ? m_parent->getClipRect().right
                                : m_canvas->getClipRect().right;
    } else {
        // Shouldn't get here as getClipping{Left|Top|Right|Bottom} should only return HIDDEN or VISIBLE.
        assert(false);
    }

    ClippingState bottom = getClippingBottom();
    if (m_position.y + m_size.y < clipRect.y + clipRect.w) {
        m_clipRect.w = clipRect.y + clipRect.w - m_clipRect.y;
    } else if (bottom == ClippingState::HIDDEN) {
        m_clipRect.w = m_position.y + m_size.y - m_clipRect.y;
    } else if (bottom == ClippingState::VISIBLE) {
        m_clipRect.w = m_parent ? m_parent->getClipRect().bottom
                                : m_canvas->getClipRect().bottom;
    } else {
        // Shouldn't get here as getClipping{Left|Top|Right|Bottom} should only return HIDDEN or VISIBLE.
        assert(false);
    }

    computeChildClipRects();
}

void vui::IWidget::calculateChildClipRects() {
    for (auto& child : m_widgets) {
        child->m_needsClipRectRecalculation = false;
        child->calculateClipRect();
    }
}

void vui::IWidget::updateChildDimensions() {
    for (auto& child : m_widgets) {
        child->m_needsDimensionUpdate = false;
        child->updateDimensions();
    }
}

void vui::IWidget::updateChildCanvases() {
    for (auto& child : m_widgets) {
        child->m_canvas = m_canvas;
        child->updateChildCanvases();
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
    m_needsDimensionUpdate = true;
}