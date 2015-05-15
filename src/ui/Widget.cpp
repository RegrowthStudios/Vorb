#include "stdafx.h"
#include "UI/Widget.h"
#include "UI/InputDispatcher.h"
#include "UI/UIRenderer.h"

vui::Widget::Widget() : IWidgetContainer() {
    enable();
    m_anchor = {};
}

vui::Widget::Widget(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : IWidgetContainer(name, destRect) {
    enable();
}

vui::Widget::Widget(IWidgetContainer* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Widget(name, destRect) {
    setParent(parent);
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    IWidgetContainer::dispose();
    removeDrawables();
}

void vui::Widget::removeDrawables() {
    if (m_renderer) {
        m_renderer->remove(this);
        m_renderer = nullptr;
    }
}

void vui::Widget::updatePosition() {
    f32v2 newPos = m_relativePosition;
    if (m_parent) {
        newPos += m_parent->getPosition();
    }
    m_position = newPos;

    computeClipRect(m_parent->getClipRect());

    // Update child positions
    for (auto& w : m_widgets) {
        w->updatePosition();
    }
}

void vorb::ui::Widget::setAnchor(const AnchorStyle& anchor) {
    m_anchor = anchor;
}

void vorb::ui::Widget::setDock(const DockStyle& dock) {
    if (m_parent) {
        m_parent->setChildDock(this, dock);
    } else {
        m_dock = dock;
    }
}

void vorb::ui::Widget::setParent(IWidgetContainer* parent) {
    if (m_parent) m_parent->removeWidget(this);
    if (parent) parent->addWidget(this);
}
