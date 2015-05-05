#include "stdafx.h"
#include "UI/Drawables.h"

void vui::DrawableRect::draw(vg::SpriteBatch* spriteBatch) const {
    spriteBatch->draw(m_texture, m_position, m_dimensions, m_style.backgroundColor, m_layerDepth);
}

void vui::DrawableText::draw(vg::SpriteBatch* spriteBatch) const {
    if (m_font) spriteBatch->drawString(m_font, m_text.c_str(), m_position, m_scale, m_style.textColor, m_textAlign, m_layerDepth, m_clipRect);
}
