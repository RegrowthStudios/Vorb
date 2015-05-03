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
}
