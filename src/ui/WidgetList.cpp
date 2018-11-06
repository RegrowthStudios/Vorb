#include "Vorb/stdafx.h"
#include "Vorb/ui/WidgetList.h"

vui::WidgetList::WidgetList() :
    Widget(),
    m_spacing(10.0f) {
    // Empty
}

vui::WidgetList::~WidgetList() {
    // Empty
}

void vui::WidgetList::initBase() {
    m_panel.init(this, getName() + "_panel");
}

void vui::WidgetList::dispose() {
    Widget::dispose();

    IWidgets().swap(m_items);
}

void vui::WidgetList::calculateDrawables() {
    f32 totalHeight = 0.0f;

    for (size_t i = 0; i < m_items.size(); ++i) {
        m_items[i]->setPosition(f32v2(0.0f, totalHeight + i * m_spacing));

        totalHeight += m_items[i]->getHeight();
    }
}

void vui::WidgetList::addItem(IWidget* item) {
    m_panel.addWidget(item);
    
    m_items.push_back(item);
    
    m_flags.needsDrawableRecalculation = true;
}

bool vui::WidgetList::addItemAtIndex(size_t index, IWidget* item) {
    if (index > m_items.size()) return false;

    m_items.insert(m_items.begin() + index, item);

    m_panel.addWidget(item);

    m_flags.needsDrawableRecalculation = true;

    return true;
}

void vui::WidgetList::addItems(const IWidgets& items) {
    for (auto& item : items) {
        addItem(item);
    }
}

bool vui::WidgetList::removeItem(IWidget* item) {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        if (*it == item) {
            m_items.erase(it);

            m_panel.removeWidget(item);

            m_flags.needsDrawableRecalculation = true;

            return true;
        }
    }
    return false;
}

bool vui::WidgetList::removeItem(size_t index) {
    if (index > m_items.size()) return false;

    auto it = (m_items.begin() + index);

    m_panel.removeWidget(*it);

    m_items.erase(it);

    m_flags.needsDrawableRecalculation = true;

    return true;
}

void vui::WidgetList::setTexture(VGTexture texture) {
    m_panel.setTexture(texture);
}

void vui::WidgetList::setBackColor(const color4& color) {
    m_panel.setColor(color);
}

void vui::WidgetList::setBackHoverColor(const color4& color) {
    m_panel.setHoverColor(color);
}

void vui::WidgetList::setSpacing(f32 spacing) {
    m_spacing = spacing;

    m_flags.needsDrawableRecalculation = true;
}

void vui::WidgetList::setAutoScroll(bool autoScroll) {
    m_panel.setAutoScroll(autoScroll);
}
