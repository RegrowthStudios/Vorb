#include "stdafx.h"
#include "UI/Drawables.h"
#include "graphics/SpriteBatch.h"

void vui::DrawableRect::draw(vg::SpriteBatch* spriteBatch) const {
    spriteBatch->draw(m_texture, m_position, m_dimensions, m_style.backgroundColor, m_layerDepth);
}

void vui::DrawableText::draw(vg::SpriteBatch* spriteBatch) const {
    spriteBatch->drawString(m_font, m_text.c_str(), m_position, m_scale, m_style.textColor, m_layerDepth);
}
