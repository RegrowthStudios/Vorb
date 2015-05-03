#include "stdafx.h"
#include "UI/Widget.h"

vui::Widget::Widget() {
    m_anchor = {};
    m_style = {};
    m_dock = DockStyle::NONE;
}

vui::Widget::Widget(const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : Widget() {
    m_name = name;
    m_destRect = destRect;
}

vui::Widget::Widget(Widget* parent, const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : Widget(name, destRect) {
    parent->addChild(this);
    m_parent = parent;
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    // TODO(Ben): Implement
}

bool vui::Widget::addChild(Widget* child) {
    m_widgets.push_back(child);
    return true; // TODO(Ben): Is this needed?
}

void vorb::ui::Widget::onMouseClick(Sender s, const MouseButtonEvent& e) {
    // TODO(Ben): Implement
}

void vorb::ui::Widget::onMouseDown(Sender s, const MouseButtonEvent& e) {
    // TODO(Ben): Implement
}

void vorb::ui::Widget::onMouseUp(Sender s, const MouseButtonEvent& e) {
    // TODO(Ben): Implement
}

void vorb::ui::Widget::onMouseEnter(Sender s, const MouseEvent& e) {
    // TODO(Ben): Implement
}

void vorb::ui::Widget::onMouseLeave(Sender s, const MouseEvent& e) {
    // TODO(Ben): Implement
}

void vorb::ui::Widget::onMouseMove(Sender s, const MouseEvent& e) {
    // TODO(Ben): Implement
}
