#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/WidgetList.h"

vui::WidgetList::WidgetList() :
    Widget(),
    m_spacing(10.0f),
    m_maxHeight(FLT_MAX) {
    // Empty
}

vui::WidgetList::~WidgetList() {
    // Empty
}

void vui::WidgetList::initBase() {
    m_panel.init(this, getName() + "_panel");

    m_panel.setClipping(vui::Clipping{ ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN });
}

void vui::WidgetList::dispose() {
    Widget::dispose();

    IWidgets().swap(m_items);
}

void vui::WidgetList::updateDimensions(f32 dt) {
    Widget::updateDimensions(dt);

    f32 totalHeight = 0.0f;

    for (size_t i = 0; i < m_items.size(); ++i) {
        IWidget* child = m_items[i];

        // We need to update the child's dimensions now, as we might otherwise get screwy scroll bars as the child isn't up-to-date with parent changes.
        {
            WidgetFlags oldFlags = child->getFlags();
            child->setFlags({
                oldFlags.isClicking,
                oldFlags.isEnabled,
                oldFlags.isMouseIn,
                oldFlags.ignoreOffset,
                true,  // needsDimensionUpdate
                false, // needsZIndexReorder
                false, // needsDockRecalculation
                false, // needsClipRectRecalculation
                false  // needsDrawableRecalculation
            });

            child->update(0.0f);

            WidgetFlags newFlags = child->getFlags();
            child->setFlags({
                newFlags.isClicking,
                newFlags.isEnabled,
                newFlags.isMouseIn,
                newFlags.ignoreOffset,
                false, // needsDimensionUpdate
                oldFlags.needsZIndexReorder         || newFlags.needsZIndexReorder,
                oldFlags.needsDockRecalculation     || newFlags.needsDockRecalculation,
                oldFlags.needsClipRectRecalculation || newFlags.needsClipRectRecalculation,
                oldFlags.needsDrawableRecalculation || newFlags.needsDrawableRecalculation
            });
        }

        child->setPosition(f32v2(0.0f, totalHeight + i * m_spacing));

        totalHeight += child->getHeight();
    }

    m_panel.setSize(f32v2(getWidth(), totalHeight > m_maxHeight ? m_maxHeight : totalHeight));
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

void vui::WidgetList::setMaxHeight(f32 maxHeight) {
    m_maxHeight = maxHeight;

    m_flags.needsDimensionUpdate = true;
}
