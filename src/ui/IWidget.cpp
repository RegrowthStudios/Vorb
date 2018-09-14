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
    m_viewport(this),
    m_parent(nullptr),
    m_widgets(IWidgets()),
    m_font(nullptr),
    m_position(f32v2(0.0f)),
    m_size(f32v2(0.0f)),
    m_clipping(DEFAULT_CLIPPING),
    m_zIndex(1),
    m_dock({ DockState::NONE, 0.0f }),
    m_name(""),
    m_flags({ false, false, false, false, false, false, false, false }) {
    // As the widget has just been made, it is its own canvas - thus should be subscribed for resize events.
    resetClipRect();
    enable();
    if (renderer) {
        setRenderer(renderer);
    } else {
        m_renderer = nullptr;
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

    removeDrawables();

    disable();
}

void vui::IWidget::update(f32 dt VORB_MAYBE_UNUSED /*= 1.0f*/) {
    if (m_flags.needsDimensionUpdate) {
        m_flags.needsDimensionUpdate = false;
        updateDimensions();
    }

    if (m_flags.needsZIndexReorder) {
        m_flags.needsZIndexReorder = false;
        reorderWidgets();
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

    updateChildren(dt);
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
    child->m_flags.needsDimensionUpdate       = true;
    child->m_flags.needsClipRectRecalculation = true;

    m_flags.needsZIndexReorder     = true;
    m_flags.needsDockRecalculation = true;

    return true;
}

bool vui::IWidget::removeWidget(IWidget* child) {
    for (auto it = m_widgets.begin(); it != m_widgets.end(); it++) {
        if (*it == child) {
            m_widgets.erase(it);
            m_flags.needsDockRecalculation = true;
            return true;
        }
    }
    return false;
}

bool vui::IWidget::isInBounds(f32 x, f32 y) const {
    return (x >= m_position.x && x < m_position.x + m_size.x &&
            y >= m_position.y && y < m_position.y + m_size.y);
}

void vui::IWidget::addDrawables() {
    addChildDrawables();
}

void vui::IWidget::addChildDrawables() {
    for (auto& child : m_widgets) {
        child->addDrawables();
    }
}

void vui::IWidget::removeDrawables() {
    if (m_renderer) m_renderer->remove(this);

    removeChildDrawables();
}

void vui::IWidget::removeChildDrawables() {
    for (auto& child : m_widgets) {
        child->removeDrawables();
    }
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

        if (m_viewport == this) {
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

void vui::IWidget::setDock(Dock dock) {
    m_dock = dock;

    if (m_parent) {
        m_parent->m_flags.needsDockRecalculation = true;
    }
}

void vui::IWidget::setDockState(DockState state) {
    m_dock.state = state;

    if (m_parent) {
        m_parent->m_flags.needsDockRecalculation = true;
    }
}

void vui::IWidget::setDockSize(f32 size) {
    m_dock.size = size;

    if (m_parent) {
        m_parent->m_flags.needsDockRecalculation = true;
    }
}

void vui::IWidget::updateChildren(f32 dt /*= 1.0f*/) {
    for (auto& child : m_widgets) {
        child->update(dt);
    }
}

void vui::IWidget::calculateDockedWidgets() {
    f32 surplusWidth  = m_size.x;
    f32 surplusHeight = m_size.y;
    f32 leftFill      = 0.0f;
    f32 topFill       = 0.0f;

    for (auto& child : m_widgets) {
        if (child->getDockState() == DockState::NONE) continue;

        DockState state = child->getDockState();
        f32       size  = child->getDockSize();
        switch (state) {
            case DockState::LEFT:
                if (surplusWidth > size) {
                    child->setWidth(size);
                    surplusWidth -= size;
                } else {
                    child->setWidth(surplusWidth);
                    surplusWidth = 0.0f;
                }

                child->setPosition(f32v2(leftFill, topFill));

                child->setHeight(surplusHeight);
                break;
            case DockState::TOP:
                if (surplusHeight > size) {
                    child->setHeight(size);
                    surplusHeight -= size;
                } else {
                    child->setHeight(surplusHeight);
                    surplusHeight = 0.0f;
                }

                child->setPosition(f32v2(leftFill, topFill));

                child->setWidth(surplusWidth);
                break;
            case DockState::RIGHT:
                if (surplusWidth > size) {
                    child->setWidth(size);
                    surplusWidth -= size;

                    child->setPosition(f32v2(leftFill + surplusWidth, topFill));
                } else {
                    child->setWidth(surplusWidth);
                    surplusWidth = 0.0f;

                    child->setPosition(f32v2(leftFill, topFill));
                }

                child->setHeight(surplusHeight);
                break;
            case DockState::BOTTOM:
                if (surplusHeight > size) {
                    child->setHeight(size);
                    surplusHeight -= size;

                    child->setPosition(f32v2(leftFill, topFill + surplusHeight));
                } else {
                    child->setHeight(surplusHeight);
                    surplusHeight = 0.0f;

                    child->setPosition(f32v2(leftFill, topFill));
                }

                child->setWidth(surplusWidth);
                break;
            case DockState::FILL:
                child->setSize(f32v2(surplusWidth, surplusHeight));
                child->setPosition(f32v2(leftFill, topFill));
                surplusWidth  = 0.0f;
                surplusHeight = 0.0f;
                break;
            default:
                // Shouldn't get here.
                assert(false);
                break;
        }

        if (surplusWidth == 0.0f && surplusHeight == 0.0f) break;
    }
}

void vui::IWidget::calculateClipRect() {
    // TODO(Matthew): Revisit inherit.
    f32v4 clipRect = m_parent->getClipRect();

    ClippingState left = getClippingLeft();
    if (m_position.x < clipRect.x) {
        m_clipRect.x = clipRect.x;
    } else if (left == ClippingState::HIDDEN) {
        m_clipRect.x = m_position.x;
    } else if (left == ClippingState::VISIBLE) {
        m_clipRect.x = m_parent ? m_parent->getClipRect().x
                                : m_canvas->getClipRect().x;
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
        m_clipRect.y = m_parent ? m_parent->getClipRect().y
                                : m_canvas->getClipRect().y;
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
        m_clipRect.z = m_parent ? m_parent->getClipRect().z
                                : m_canvas->getClipRect().z;
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
        m_clipRect.w = m_parent ? m_parent->getClipRect().w
                                : m_canvas->getClipRect().w;
    } else {
        // Shouldn't get here as getClipping{Left|Top|Right|Bottom} should only return HIDDEN or VISIBLE.
        assert(false);
    }

    calculateChildClipRects();
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

void vui::IWidget::reorderWidgets() {
    std::sort(m_widgets.begin(), m_widgets.end(), [](const IWidget* lhs, const IWidget* rhs) {
        return lhs->getZIndex() >= rhs->getZIndex();
    });

    reorderChildWidgets();

    setRenderer(getRenderer());
}

void vui::IWidget::reorderChildWidgets() {
    for (auto& child : m_widgets) {
        std::vector<IWidget*> grandchildren = child->getWidgets();

        std::sort(grandchildren.begin(), grandchildren.end(), [](const IWidget* lhs, const IWidget* rhs) {
            return lhs->getZIndex() >= rhs->getZIndex();
        });

        child->reorderChildWidgets();
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
