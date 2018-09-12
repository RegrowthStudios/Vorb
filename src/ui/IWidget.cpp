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
    m_flags({ false, false, false, false, false, false, false }) {
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
    if (m_flags.needsDimensionUpdate) {
        m_flags.needsDimensionUpdate = false;
        updateDimensions();
    }

    if (m_flags.needsDockRecalculation) {
        m_flags.needsDockRecalculation = false;
        calculateDockedWidgets();
    }

    if (m_flags.needsClipRectRecalculation) {
        m_flags.needsClipRectRecalculation = false;
        calculateClipRect();
    }

    if (m_flags.needsDrawableRefresh) {
        m_flags.needsDrawableRefresh = false;
        refreshDrawables();
    }
}

void vui::IWidget::enable() {
    if (!m_flags.isEnabled) {
        m_flags.isEnabled = true;
        vui::InputDispatcher::mouse.onButtonDown += makeDelegate(*this, &IWidget::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp += makeDelegate(*this, &IWidget::onMouseUp);
        vui::InputDispatcher::mouse.onMotion += makeDelegate(*this, &IWidget::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost += makeDelegate(*this, &IWidget::onMouseFocusLost);
    }
    // Enable all children
    for (auto& w : m_widgets) w->enable();
}

void vui::IWidget::disable() {
    if (m_flags.isEnabled) {
        m_flags.isEnabled = false;
        vui::InputDispatcher::mouse.onButtonDown -= makeDelegate(*this, &IWidget::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp -= makeDelegate(*this, &IWidget::onMouseUp);
        vui::InputDispatcher::mouse.onMotion -= makeDelegate(*this, &IWidget::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost -= makeDelegate(*this, &IWidget::onMouseFocusLost);
    }
    m_flags.isClicking = false;
    // Disable all children
    for (auto& w : m_widgets) w->disable();
}

bool vui::IWidget::addWidget(IWidget* child) {
    m_widgets.push_back(child);
    child->m_parent = this;
    child->m_flags.needsDimensionUpdate = true;
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

vui::ClippingState vui::IWidget::getClippingLeft() const {
    return (m_clipping.left == ClippingState::INHERIT ? m_parent->getClippingLeft() : m_clipping.left);
}

vui::ClippingState vui::IWidget::getClippingTop() const {
    return (m_clipping.top == ClippingState::INHERIT ? m_parent->getClippingTop() : m_clipping.top);
}

vui::ClippingState vui::IWidget::getClippingRight() const {
    return (m_clipping.right == ClippingState::INHERIT ? m_parent->getClippingRight() : m_clipping.right);
}

vui::ClippingState vui::IWidget::getClippingBottom() const {
    return (m_clipping.bottom == ClippingState::INHERIT ? m_parent->getClippingBottom() : m_clipping.bottom);
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
    m_flags.needsDimensionUpdate       = true;
    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setPosition(f32v2 position) {
    m_position = position;

    updateChildDimensions();

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setX(f32 x) {
    m_position.x = x;
    
    updateChildDimensions();
    
    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setY(f32 y) {
    m_position.y = y;
    
    updateChildDimensions();
    
    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setRelativePosition(f32v2 relativePosition) {
    m_position = relativePosition + m_parent->getPosition();

    updateChildDimensions();

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setRelativeX(f32 relativeX) {
    m_position.x = relativeX + m_parent->getPosition().x;

    updateChildDimensions();

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setRelativeY(f32 relativeY) {
    m_position.y = relativeY + m_parent->getPosition().y;

    updateChildDimensions();

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setSize(f32v2 size) {
    m_size = size;

    updateChildDimensions();

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setWidth(f32 width) {
    m_size.x = width;

    updateChildDimensions();

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setHeight(f32 height) {
    m_size.y = height;
    
    updateChildDimensions();

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setClipping(Clipping clipping) {
    m_clipping = clipping;

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setClippingLeft(ClippingState state) {
    m_clipping.left = state;

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setClippingTop(ClippingState state) {
    m_clipping.top = state;

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setClippingRight(ClippingState state) {
    m_clipping.right = state;

    m_flags.needsClipRectRecalculation = true;
}

void vui::IWidget::setClippingBottom(ClippingState state) {
    m_clipping.bottom = state;

    m_flags.needsClipRectRecalculation = true;
}


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
        child->m_flags.needsClipRectRecalculation = false;
        child->calculateClipRect();
    }
}

void vui::IWidget::updateChildDimensions() {
    for (auto& child : m_widgets) {
        child->m_flags.needsDimensionUpdate = false;
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
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        MouseDown(e);
        m_flags.isClicking = true;
    }
}

void vui::IWidget::onMouseUp(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        MouseUp(e);
        if (m_flags.isClicking) MouseClick(e);
    }
    m_flags.isClicking = false;
}

void vui::IWidget::onMouseMove(Sender s VORB_UNUSED, const MouseMotionEvent& e) {
    if (!m_flags.isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_flags.isMouseIn) {
            m_flags.isMouseIn = true;
            MouseEnter(e);
        }
        MouseMove(e);
    } else if (m_flags.isMouseIn) {
        m_flags.isMouseIn = false;
        MouseLeave(e);
    }
}

void vui::IWidget::onMouseFocusLost(Sender s VORB_UNUSED, const MouseEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        m_flags.isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
    }
}

void vui::IWidget::onResize(Sender s VORB_MAYBE_UNUSED, const WindowResizeEvent& e VORB_MAYBE_UNUSED) {
    m_flags.needsDimensionUpdate = true;
}