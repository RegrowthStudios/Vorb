#include "Vorb/stdafx.h"
#include "Vorb/ui/Form.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/GameWindow.h"

vui::Form::Form() : IWidget() {
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
    m_size.x = destRect.z;
    m_size.y = destRect.w;
    m_renderer.init(defaultFont, spriteBatch);
}

bool vui::Form::addWidget(IWidget* widget) {
    if (IWidget::addWidget(widget)) {
        if (!widget->getRenderer()) widget->addDrawables(&m_renderer);
        return true;
    }
    return false;
}

bool vui::Form::removeWidget(IWidget* widget) {
    if (IWidget::removeWidget(widget)) {
        return true;
    }
    return false;
}

// void vui::Form::updatePosition() {
//     m_position = m_relativePosition;

//     computeClipRect();
    
//     updateChildPositions();
// }

void vui::Form::update(VORB_UNUSED f32 dt /*= 1.0f*/) {
    if (!m_isEnabled) return;
    for (VORB_UNUSED auto& w : m_widgets) {
        // Check if we need to reload the drawables
        // if (w->needsDrawableReload()) {
        //     w->removeDrawables();
        //     w->setNeedsDrawableReload(false);
        //     w->addDrawables(&m_renderer);
        // }
        // w->update(dt);
    }
}

void vui::Form::draw() {
    if (!m_isEnabled) return;
    m_renderer.draw(m_size);
}

void vui::Form::dispose() {
    IWidget::dispose();
    m_renderer.dispose();
}
