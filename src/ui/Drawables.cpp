#include "Vorb/stdafx.h"
#include "Vorb/UI/Drawables.h"
#include "Vorb/utils.h"

void vui::DrawableRect::draw(vg::SpriteBatch* spriteBatch) const {
    f32v4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    // Check clipping
    f32v2 pos = m_position;
    f32v2 dims = m_dimensions;
    computeClipping(m_clipRect, pos, dims, uvRect);

    // Draw it
    if (dims.x > 0 && dims.y > 0) {
        spriteBatch->draw(m_texture, &uvRect, pos, dims, m_color1, m_color2, m_grad, m_layerDepth);
    }
}

void vui::DrawableText::draw(vg::SpriteBatch* spriteBatch) const {
    if (m_font) spriteBatch->drawString(m_font, m_text.c_str(), m_position, m_scale, m_color1, m_textAlign, m_layerDepth, m_clipRect, true);
}
