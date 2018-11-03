#include "Vorb/stdafx.h"
#include "Vorb/ui/Label.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/utils.h"

vui::Label::Label() :
    Widget(),
    m_defaultFont(nullptr) {
    m_flags.needsDrawableRecalculation = true;
}

vui::Label::~Label() {
    // Empty
}

void vui::Label::addDrawables() {
    // Make copies.
    m_drawnText = m_drawableText;

    // Use renderer default font if we dont have a font.
    m_defaultFont = m_viewport->getRenderer()->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the text.
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &Label::refreshDrawables));
}

void vui::Label::setFont(const vg::SpriteFont* font) {
    m_drawableText.setFont(font);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Label::setText(const nString& text) {
    m_drawableText.setText(text);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Label::setTextColor(const color4& color) {
    m_drawableText.setColor(color);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Label::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Label::calculateDrawables() {
    m_drawableText.setClipRect(m_clipRect);

    updateTextPosition();

    m_flags.needsDrawableRefresh = true;
}

void vui::Label::updateTextPosition() {
    const f32v2& dims = getSize();
    const f32v2& pos  = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();

    switch (textAlign) {
        case vg::TextAlign::LEFT:
            m_drawableText.setPosition(pos + f32v2(0.0f, dims.y / 2.0f));
            break;
        case vg::TextAlign::TOP_LEFT:
            m_drawableText.setPosition(pos);
            break;
        case vg::TextAlign::TOP:
            m_drawableText.setPosition(pos + f32v2(dims.x / 2.0f, 0.0f));
            break;
        case vg::TextAlign::TOP_RIGHT:
            m_drawableText.setPosition(pos + f32v2(dims.x, 0.0f));
            break;
        case vg::TextAlign::RIGHT:
            m_drawableText.setPosition(pos + f32v2(dims.x, dims.y / 2.0f));
            break;
        case vg::TextAlign::BOTTOM_RIGHT:
            m_drawableText.setPosition(pos + f32v2(dims.x, dims.y));
            break;
        case vg::TextAlign::BOTTOM:
            m_drawableText.setPosition(pos + f32v2(dims.x / 2.0f, dims.y));
            break;
        case vg::TextAlign::BOTTOM_LEFT:
            m_drawableText.setPosition(pos + f32v2(0.0f, dims.y));
            break;
        case vg::TextAlign::CENTER:
            m_drawableText.setPosition(pos + dims / 2.0f);
            break;
    }
}

void vui::Label::refreshDrawables() {
    // Use renderer default font if we don't have a font.
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);

        m_drawnText = m_drawableText;

        m_drawableText.setFont(nullptr);
    } else {
        m_drawnText = m_drawableText;
    }
}
