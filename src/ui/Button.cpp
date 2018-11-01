#include "Vorb/stdafx.h"
#include "Vorb/ui/Button.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"

vui::Button::Button() : Button("") {
    // Empty
}

vui::Button::Button(const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/) : Widget(name, dimensions) {
    m_gradBack        = vg::GradientType::NONE;
    m_gradHover       = vg::GradientType::NONE;
    m_backColor1      = color::LightGray;
    m_backColor2      = color::LightGray;
    m_backHoverColor1 = color::AliceBlue;
    m_backHoverColor2 = color::AliceBlue;
    m_textColor       = color::Black;
    m_textHoverColor  = color::Black;
    m_defaultFont     = nullptr;

    m_flags.needsDrawableRecalculation = true;
}

vui::Button::Button(const nString& name, const Length2& position, const Length2& size) : Widget(name, position, size) {
    m_gradBack        = vg::GradientType::NONE;
    m_gradHover       = vg::GradientType::NONE;
    m_backColor1      = color::LightGray;
    m_backColor2      = color::LightGray;
    m_backHoverColor1 = color::AliceBlue;
    m_backHoverColor2 = color::AliceBlue;
    m_textColor       = color::Black;
    m_textHoverColor  = color::Black;
    m_defaultFont     = nullptr;

    m_flags.needsDrawableRecalculation = true;
}

vui::Button::Button(IWidget* parent, const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/) : Button(name, dimensions) {
    parent->addWidget(this);
}

vui::Button::Button(IWidget* parent, const nString& name, const Length2& position, const Length2& size) : Button(name, position, size) {
    parent->addWidget(this);
}

vui::Button::~Button() {
    // Empty
}

void vui::Button::addDrawables() {
    // Make copies
    m_drawnText = m_drawableText;
    m_drawnRect = m_drawableRect;
    // Use renderer default font if we dont have a font
    m_defaultFont = m_viewport->getRenderer()->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the rect
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &Button::refreshDrawables));
    
    // Add the text 
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &Button::refreshDrawables));  
}

void vui::Button::setFont(const vg::SpriteFont* font) {
    m_drawableText.setFont(font);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Button::setTexture(VGTexture texture) {
    m_drawableRect.setTexture(texture);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Button::setBackColor(const color4& color) {
    m_backColor1 = m_backColor2 = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setBackColorGrad(const color4& color1, const color4& color2, vg::GradientType grad) {
    m_backColor1 = color1;
    m_backColor2 = color2;
    m_gradBack = grad;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setBackHoverColor(const color4& color) {
    m_backHoverColor1 = m_backHoverColor2 = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setBackHoverColorGrad(const color4& color1, const color4& color2, vg::GradientType grad) {
    m_backHoverColor1 = color1;
    m_backHoverColor2 = color2;
    m_gradHover = grad;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setText(const nString& text) {
    m_drawableText.setText(text);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Button::setTextColor(const color4& color) {
    m_textColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setTextHoverColor(const color4& color) {
    m_textHoverColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::Button::calculateDrawables() {
    m_drawableRect.setPosition(m_position);
    m_drawableRect.setSize(m_size);
    m_drawableRect.setClipRect(m_clipRect);
    m_drawableText.setClipRect(m_clipRect);

    updateColor();

    updateTextPosition();

    m_flags.needsDrawableRefresh = true;
}

void vui::Button::updateColor() {
    if (m_flags.isMouseIn) {
        m_drawableRect.setColor1(m_backHoverColor1);
        m_drawableRect.setColor2(m_backHoverColor2);
        m_drawableRect.setGradientType(m_gradBack);
        m_drawableText.setColor(m_textHoverColor);
    } else {
        m_drawableRect.setColor1(m_backColor1);
        m_drawableRect.setColor2(m_backColor2);
        m_drawableRect.setGradientType(m_gradHover);
        m_drawableText.setColor(m_textColor);
    }
}

// TODO(Matthew): This is /very/ repeatable across any widget with text - we should move it out of any specific widget class.
void vui::Button::updateTextPosition() {
    const f32v2& dims = getSize();
    const f32v2& pos  = getPosition();

    const vg::TextAlign& textAlign = getTextAlign();

    // TODO(Ben): Padding
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

// TODO(Matthew): This seems a bit repeatable too.
void vui::Button::refreshDrawables() {
    // Use renderer default font if we don't have a font.
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);

        m_drawnText = m_drawableText;

        m_drawableText.setFont(nullptr);
    } else {
        m_drawnText = m_drawableText;
    }
    
    m_drawnRect = m_drawableRect;
}

void vui::Button::onMouseMove(Sender s VORB_MAYBE_UNUSED, const MouseMotionEvent& e) {
    if (!m_flags.isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_flags.isMouseIn) {
            m_flags.isMouseIn = true;
            MouseEnter(e);

            updateColor();

            m_flags.needsDrawableRefresh = true;
        }
        MouseMove(e);
    } else {        
        if (m_flags.isMouseIn) {
            m_flags.isMouseIn = false;
            MouseLeave(e);

            updateColor();

            m_flags.needsDrawableRefresh = true;
        }
    }
}

void vui::Button::onMouseFocusLost(Sender s VORB_MAYBE_UNUSED, const MouseEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        m_flags.isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
        updateColor();
    }
}
