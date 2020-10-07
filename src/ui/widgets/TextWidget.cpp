#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/TextWidget.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"

vui::TextWidget::TextWidget() : Widget() {
    m_flags.needsDrawableRecalculation = true;
}

vui::TextWidget::~TextWidget() {
    // Empty
}

void vui::TextWidget::addDrawables(UIRenderer& renderer) {
    auto performAdd = [&renderer](DrawableText& text) {
        // Use renderer default font if we dont have a font.
        if (!text.getFont()) {
            text.setFont(renderer.getDefaultFont());
            // Add the text.
            renderer.add(makeDelegate(&text, &DrawableText::draw));
            text.setFont(nullptr);
        } else {
            // Add the text.
            renderer.add(makeDelegate(&text, &DrawableText::draw));
        }
    };

    if (m_flags.isMouseIn) {
        performAdd(m_drawableText);
    } else {
        performAdd(m_drawableHoverText);
    }
}

void vui::TextWidget::setFont(const vg::SpriteFont* font) {
    m_drawableText.setFont(font);

    // Hover should match non-hover property unless explicitly modified.
    if (!m_isModifiedMap["hoverFont"]) m_drawableHoverText.setFont(font);
}

void vui::TextWidget::setText(const nString& text) {
    m_drawableText.setText(text);

    // Hover should match non-hover property unless explicitly modified.
    if (!m_isModifiedMap["hoverText"]) m_drawableHoverText.setText(text);
}

void vui::TextWidget::setTextColor(const color4& color) {
    m_drawableText.setColor(color);

    // Hover should match non-hover property unless explicitly modified.
    if (!m_isModifiedMap["hoverTextColor"]) m_drawableHoverText.setColor(color);
}

void vui::TextWidget::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);

    // Hover should match non-hover property unless explicitly modified.
    if (!m_isModifiedMap["hoverTextAlign"]) m_drawableHoverText.setTextAlign(textAlign);

    m_flags.needsDrawableRecalculation = true;
}

void vui::TextWidget::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);

    // Hover should match non-hover property unless explicitly modified.
    if (!m_isModifiedMap["hoverTextScale"]) m_drawableHoverText.setTextScale(textScale);
}

void vui::TextWidget::setHoverFont(const vg::SpriteFont* font) {
    m_isModifiedMap["hoverFont"] = true;

    m_drawableHoverText.setFont(font);
}

void vui::TextWidget::setHoverText(const nString& text) {
    m_isModifiedMap["hoverText"] = true;

    m_drawableHoverText.setText(text);
}

void vui::TextWidget::setHoverTextColor(const color4& color) {
    m_isModifiedMap["hoverTextColor"] = true;

    m_drawableHoverText.setColor(color);
}

void vui::TextWidget::setHoverTextAlign(vg::TextAlign textAlign) {
    m_isModifiedMap["hoverTextAlign"] = true;

    m_drawableHoverText.setTextAlign(textAlign);

    m_flags.needsDrawableRecalculation = true;
}

void vui::TextWidget::setHoverTextScale(const f32v2& textScale) {
    m_isModifiedMap["hoverTextScale"] = true;

    m_drawableHoverText.setTextScale(textScale);
}

void vui::TextWidget::initBase() {
    Widget::initBase();

    m_inheritableGetterSetterMap["font"] = {
        makeFunctor([](IWidget* widget) {
            return any(((TextWidget*)widget)->getFont());
        }),
        makeFunctor([](IWidget* widget, any font) {
            ((TextWidget*)widget)->setFont(any_cast<const vg::SpriteFont*>(font));
        })
    };
    m_isModifiedMap["font"] = false;
    m_inheritableGetterSetterMap["textColor"] = {
        makeFunctor([](IWidget* widget) {
            return any(((TextWidget*)widget)->getTextColor());
        }),
        makeFunctor([](IWidget* widget, any textColor) {
            ((TextWidget*)widget)->setTextColor(any_cast<color4>(textColor));
        })
    };
    m_isModifiedMap["textColor"] = false;
    m_inheritableGetterSetterMap["textAlign"] = {
        makeFunctor([](IWidget* widget) {
            return any(((TextWidget*)widget)->getTextAlign());
        }),
        makeFunctor([](IWidget* widget, any textAlign) {
            ((TextWidget*)widget)->setTextAlign(any_cast<vg::TextAlign>(textAlign));
        })
    };
    m_isModifiedMap["textAlign"] = false;
    m_inheritableGetterSetterMap["textScale"] = {
        makeFunctor([](IWidget* widget) {
            return any(((TextWidget*)widget)->getTextScale());
        }),
        makeFunctor([](IWidget* widget, any textScale) {
            ((TextWidget*)widget)->setTextScale(any_cast<f32v2>(textScale));
        })
    };
    m_isModifiedMap["textScale"] = false;

    m_isModifiedMap["hoverFont"] = false;
    m_isModifiedMap["hoverText"] = false;
    m_isModifiedMap["hoverTextColor"] = false;
    m_isModifiedMap["hoverTextAlign"] = false;
    m_isModifiedMap["hoverTextScale"] = false;
}

void vui::TextWidget::calculateDrawables() {
    m_drawableText.setClipRect(m_clipRect);
    m_drawableHoverText.setClipRect(m_clipRect);

    updateTextPosition();
}

void vui::TextWidget::updateTextPosition() {
    const f32v2& size = getSize();
    const f32v2& pos  = getPosition();

    auto performUpdate = [pos, size](DrawableText& text, const vg::TextAlign& align) {
        switch (align) {
            case vg::TextAlign::LEFT:
                text.setPosition(pos + f32v2(0.0f, size.y / 2.0f));
                break;
            case vg::TextAlign::TOP_LEFT:
                text.setPosition(pos);
                break;
            case vg::TextAlign::TOP:
                text.setPosition(pos + f32v2(size.x / 2.0f, 0.0f));
                break;
            case vg::TextAlign::TOP_RIGHT:
                text.setPosition(pos + f32v2(size.x, 0.0f));
                break;
            case vg::TextAlign::RIGHT:
                text.setPosition(pos + f32v2(size.x, size.y / 2.0f));
                break;
            case vg::TextAlign::BOTTOM_RIGHT:
                text.setPosition(pos + f32v2(size.x, size.y));
                break;
            case vg::TextAlign::BOTTOM:
                text.setPosition(pos + f32v2(size.x / 2.0f, size.y));
                break;
            case vg::TextAlign::BOTTOM_LEFT:
                text.setPosition(pos + f32v2(0.0f, size.y));
                break;
            case vg::TextAlign::CENTER:
                text.setPosition(pos + size / 2.0f);
                break;
            default:
                assert(false);
                break;
        }
    };

    performUpdate(m_drawableText, getTextAlign());
    performUpdate(m_drawableHoverText, getHoverTextAlign());
}
