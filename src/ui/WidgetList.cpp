#include "stdafx.h"
#include "ui/WidgetList.h"

vui::WidgetList::WidgetList() : Widget() {
    addWidget(&m_panel);
}

vui::WidgetList::WidgetList(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : WidgetList() {
    m_name = name;
    setDestRect(destRect);
    m_panel.setDimensions(m_dimensions);
    updatePosition();
}

vui::WidgetList::WidgetList(IWidgetContainer* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : WidgetList(name, destRect) {
    parent->addWidget(this);
}

vui::WidgetList::~WidgetList() {
    // Empty
}

void vui::WidgetList::dispose() {
    Widget::dispose();
    std::vector<Widget*>().swap(m_listedWidgets);
}

void vui::WidgetList::updatePosition() {
    f32 totalHeight = 0.0f;
    for (size_t i = 0; i < m_listedWidgets.size(); i++) {
        m_listedWidgets[i]->setPosition(f32v2(0.0f, totalHeight + i * m_spacing));
        totalHeight += m_listedWidgets[i]->getHeight();
    }
    vui::Widget::updatePosition();
}

void vui::WidgetList::addItem(Widget* w) {
    m_listedWidgets.push_back(w);
    updatePosition();
}

bool vui::WidgetList::addItemAtIndex(int index, Widget* w) {
    if (index > (int)m_listedWidgets.size()) return false;
    m_listedWidgets.insert(m_listedWidgets.begin() + index, w);
    updatePosition();
    return true;
}

bool vui::WidgetList::removeItem(Widget* w) {
    for (auto& it = m_listedWidgets.begin(); it != m_listedWidgets.end(); it++) {
        if (*it == w) {
            w->removeDrawables();
            m_listedWidgets.erase(it);
            updatePosition();
            return true;
        }
    }
    return false;
}

bool vui::WidgetList::removeItem(int index) {
    if (index > (int)m_listedWidgets.size()) return false;
    auto& it = (m_listedWidgets.begin() + index);
    (*it)->removeDrawables();
    m_listedWidgets.erase(it);
    updatePosition();
    return true;
}

void vui::WidgetList::addItems(const std::vector <Widget*>& widgetsToAdd) {
    for (auto& it : widgetsToAdd) {
        m_listedWidgets.push_back(w);
    }
    updatePosition();
}

void vui::WidgetList::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_panel.setDimensions(dimensions);
    updatePosition();
}

void vui::WidgetList::setHeight(f32 height) {
    Widget::setHeight(height);
    m_panel.setHeight(height);
    updatePosition();
}

void vui::WidgetList::setTexture(VGTexture texture) {
    m_panel.setTexture(texture);
}

void vui::WidgetList::setWidth(f32 width) {
    Widget::setWidth(width);
    m_panel.setWidth(width);
    updatePosition();
}

void vui::WidgetList::setBackColor(const color4& color) {
    m_panel.setColor(color);
}

void vui::WidgetList::setBackHoverColor(const color4& color) {
    m_panel.setHoverColor(color);
}

void vui::WidgetList::setSpacing(f32 spacing) {
    m_spacing = spacing;
    updatePosition();
}
