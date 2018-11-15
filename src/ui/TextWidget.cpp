#include "Vorb/stdafx.h"
#include "Vorb/ui/TextWidget.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"

vui::TextWidget::TextWidget() :
    Widget(),
    m_defaultFont(nullptr) {
    m_flags.needsDrawableRecalculation = true;
}

vui::TextWidget::~TextWidget() {
    // Empty
}

void vui::TextWidget::addDrawables(UIRenderer& renderer) {
    // Use renderer default font if we dont have a font.
    m_defaultFont = renderer.getDefaultFont();
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);
        // Add the text.
        renderer.add(makeDelegate(&m_drawableText, &DrawableText::draw));
        m_drawableText.setFont(nullptr);
    } else {
        // Add the text.
        renderer.add(makeDelegate(&m_drawableText, &DrawableText::draw));
    }
}

void vui::TextWidget::setFont(const vg::SpriteFont* font) {
    m_drawableText.setFont(font);
}

void vui::TextWidget::setText(const nString& text) {
    m_drawableText.setText(text);
}

void vui::TextWidget::setTextColor(const color4& color) {
    m_drawableText.setColor(color);
}

void vui::TextWidget::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);

    m_flags.needsDrawableRecalculation = true;
}

void vui::TextWidget::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);
}

void vui::TextWidget::calculateDrawables() {
    m_drawableText.setClipRect(m_clipRect);

    updateTextPosition();
}

void vui::TextWidget::updateTextPosition() {
    const f32v2& size = getSize();
    const f32v2& pos  = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();

    switch (textAlign) {
        case vg::TextAlign::LEFT:
            m_drawableText.setPosition(pos + f32v2(0.0f, size.y / 2.0f));
            break;
        case vg::TextAlign::TOP_LEFT:
            m_drawableText.setPosition(pos);
            break;
        case vg::TextAlign::TOP:
            m_drawableText.setPosition(pos + f32v2(size.x / 2.0f, 0.0f));
            break;
        case vg::TextAlign::TOP_RIGHT:
            m_drawableText.setPosition(pos + f32v2(size.x, 0.0f));
            break;
        case vg::TextAlign::RIGHT:
            m_drawableText.setPosition(pos + f32v2(size.x, size.y / 2.0f));
            break;
        case vg::TextAlign::BOTTOM_RIGHT:
            m_drawableText.setPosition(pos + f32v2(size.x, size.y));
            break;
        case vg::TextAlign::BOTTOM:
            m_drawableText.setPosition(pos + f32v2(size.x / 2.0f, size.y));
            break;
        case vg::TextAlign::BOTTOM_LEFT:
            m_drawableText.setPosition(pos + f32v2(0.0f, size.y));
            break;
        case vg::TextAlign::CENTER:
            m_drawableText.setPosition(pos + size / 2.0f);
            break;
    }
}
