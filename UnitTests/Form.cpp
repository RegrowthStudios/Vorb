#include "stdafx.h"
#include "Form.h"
#include "Widget.h"
#include <include/ui/GameWindow.h>

vui::Form::Form() : WidgetContainer() {
    // Empty
}

vui::Form::~Form() {
    // Empty
}

void vui::Form::init(const nString& name, IGameScreen* ownerScreen, const f32v4& destRect, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_name = name;
    m_ownerIGameScreen = ownerScreen;
    m_position.x = destRect.x;
    m_position.y = destRect.y;
    m_dimensions.x = destRect.z;
    m_dimensions.y = destRect.w;
    m_renderer.init(defaultFont, spriteBatch);
}

void vui::Form::addWidget(Widget* widget) {
    WidgetContainer::addWidget(widget);

    if (!widget->getRenderer()) 
        widget->addDrawables(&m_renderer);
}

void vui::Form::removeWidget(Widget* widget) {
    WidgetContainer::removeWidget(widget);
}

void vui::Form::update(int flags, f32 dt) {
    if (flags & 0x01) {
        m_position = m_relativePosition;

        computeClipRect();
        updateChilds(flags);
    }

    if (flags & 0x04) {
        if (!isEnabled()) return;

        for (auto& w : m_widgets) {
            if (w->needsDrawableReload()) {
                w->removeDrawables();
                w->setNeedsDrawableReload(false);
                w->addDrawables(&m_renderer);
            }

            w->update(dt);
        }
    }
}

void vui::Form::draw() {
    if (!isEnabled()) return;

    m_renderer.draw(m_dimensions);
}

void vui::Form::dispose() {
    WidgetContainer::dispose();

    m_renderer.dispose();
}
