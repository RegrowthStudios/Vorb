#include "stdafx.h"
#include "ui/ComboBox.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"

vui::ComboBox::ComboBox() : Widget() {
    updateColor();
}

vui::ComboBox::ComboBox(const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : ComboBox() {
    m_name = name;
    m_destRect = destRect;
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    updateTextPosition();
}

vui::ComboBox::ComboBox(Widget* parent, const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : ComboBox(name, destRect) {
    parent->addChild(this);
    m_parent = parent;
}

vui::ComboBox::~ComboBox() {
    // Empty
}

void vui::ComboBox::addDrawables(UIRenderer* renderer) {
    removeDrawables(renderer);

    m_defaultFont = renderer->getDefaultFont();
    // Make copies
    for (auto& it : m_drawableTexts) {
        it.second = it.first;
        if (!it.second.getFont()) it.second.setFont(m_defaultFont);
    }
    m_drawnRect = m_drawableRect;
    m_drawableDropList = m_drawableDropList;
    m_drawnText = m_drawableText;
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &ComboBox::refreshDrawables));

    // Add the drop list
    renderer->add(this,
                  makeDelegate(m_drawnDropList, &DrawableRect::draw),
                  makeDelegate(*this, &ComboBox::refreshDrawables));

    // Add box text
  /*  renderer->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &ComboBox::refreshDrawables));*/

    // Add the drop down texts
    for (auto& it : m_drawableTexts) {
        renderer->add(this,
                      makeDelegate(it.second, &DrawableText::draw),
                      makeDelegate(*this, &ComboBox::refreshDrawables));
    }
}

void vui::ComboBox::removeDrawables(UIRenderer* renderer) {
    renderer->remove(this);
}

void vui::ComboBox::addItem(const nString& item) {
    m_items.push_back(item);
    m_drawableTexts.emplace_back();
    auto& dit = m_drawableTexts.back();
    dit.first.setText(item);
    updateTextPosition();
}

bool vui::ComboBox::addItemAtIndex(int index, const nString& item) {
    if (index > (int)m_items.size()) return false;
    m_items.insert(m_items.begin() + index, item);
    return true;
}

bool vui::ComboBox::removeItem(const nString& item) {
    for (auto& it = m_items.begin(); it != m_items.end(); it++) {
        if (*it == item) {
            m_items.erase(it);
            return true;
        }
    }
    return false;
}

bool vui::ComboBox::removeItem(int index) {
    if (index > (int)m_items.size()) return false;

    m_items.erase(m_items.begin() + index);
  
    return true;
}

void vui::ComboBox::addItems(const std::vector <nString>& itemsToAdd) {
    for (auto& it : itemsToAdd) {
        addItem(it);
    }
}

bool vui::ComboBox::selectItem(int index) {
    if (index > m_items.size()) return false;
    m_selected = index;
    refreshDrawables();
    return true;
}

const vorb::graphics::SpriteFont* vui::ComboBox::getFont() const {
    return m_drawableText.getFont();
}

f32v2 vui::ComboBox::getTextScale() const {
    return m_drawableText.getTextScale();
}

void vui::ComboBox::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
    updateTextPosition();
}

void vui::ComboBox::setFont(vorb::graphics::SpriteFont* font) {
    m_drawableText.setFont(font);
    for (auto& it : m_drawableTexts) {
        it.first.setFont(font);
    }
    refreshDrawables();
}

void vui::ComboBox::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);
    updateTextPosition();
}

void vui::ComboBox::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(position);
    updateTextPosition();
}

void vui::ComboBox::setTexture(VGTexture texture) {
    m_drawableRect.setTexture(texture);
    m_drawableDropList.setTexture(texture);
    refreshDrawables();
}

void vui::ComboBox::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);
    updateTextPosition();
}

void vui::ComboBox::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(x);
    updateTextPosition();
}

void vui::ComboBox::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(y);
    updateTextPosition();
}

void vui::ComboBox::setBackColor(const color4& color) {
    m_backColor = color;
    updateColor();
}

void vui::ComboBox::setBackHoverColor(const color4& color) {
    m_backHoverColor = color;
    updateColor();
}

void vui::ComboBox::setTextColor(const color4& color) {
    m_textColor = color;
    updateColor();
}

void vui::ComboBox::setTextHoverColor(const color4& color) {
    m_textHoverColor = color;
    updateColor();
}

void vui::ComboBox::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);
    for (auto& it : m_drawableTexts) {
        it.first.setTextScale(textScale);
    }
    refreshDrawables();
}

void vui::ComboBox::updateColor() {
    if (m_isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
        m_drawableText.setColor(m_textHoverColor);
        for (auto& it : m_drawableTexts) {
            it.first.setColor(m_textHoverColor);
        }
    } else {
        m_drawableRect.setColor(m_backColor);
        m_drawableText.setColor(m_textColor);
        for (auto& it : m_drawableTexts) {
            it.first.setColor(m_textColor);
        }
    }
    refreshDrawables();
}

void vui::ComboBox::updateTextPosition() {
    const f32v2& dims = getDimensions();
    const f32v2& pos = getPosition();

    m_drawableText.setClipRect(getDestRect());
    m_drawableText.setPosition(pos);
    ui32 i = 1;
    for (auto& it : m_drawableTexts) {
        if (m_isDropped) {
            it.first.setClipRect(getDestRect() + f32v4(0.0f, i * getHeight(), 0.0f, 0.0f));
        } else {
            it.first.setClipRect(f32v4(0.0f));
        }
        it.first.setPosition(pos + f32v2(0.0f, i * getHeight()));
        i++;
    }
       
    refreshDrawables();
}

void vui::ComboBox::refreshDrawables() {
    // Drop list
    if (m_isDropped) {
        m_drawableDropList.setPosition(getPosition() + f32v2(0.0f, getHeight()));
        m_drawableDropList.setDimensions(getDimensions() * f32v2(1.0f, m_items.size()));
    } else {
        m_drawableDropList.setDimensions(f32v2(0.0f));
    }
    m_drawnDropList = m_drawableDropList;

    const f32v2& dPos = m_drawnDropList.getPosition();
    const f32v2& dDims = m_drawnDropList.getDimensions();
    const f32v4 clipRect(dPos.x, dPos.y, dDims.x, dDims.y);

    // Texts
    for (auto& it : m_drawableTexts) {
        // Use renderer default font if we don't have a font
        if (!it.first.getFont()) {
            it.first.setFont(m_defaultFont);
            it.second = it.first;
            it.first.setFont(nullptr);
        } else {
            it.second = it.first;
        }
    }

    m_drawnRect = m_drawableRect;

    if (m_selected > -1) {
        m_drawableText.setText(m_items[m_selected]);
    }
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);
        m_drawnText = m_drawableText;
        m_drawableText.setFont(nullptr);
    } else {
        m_drawnText = m_drawableText;
    }  
}

void vui::ComboBox::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) {
            MouseClick(e);
            m_isDropped = !m_isDropped;
            updateTextPosition();
        } else if (m_isDropped) {
            m_isDropped = false;
            updateTextPosition();
        }
    } else if (m_isDropped) {
        m_isDropped = false;
        updateTextPosition();
    }
    m_isClicking = false;
}

void vui::ComboBox::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (isInBounds(e.x, e.y)) {
        if (!m_isMouseIn) {
            m_isMouseIn = true;
            MouseEnter(e);
            updateColor();
        }
        MouseMove(e);
    } else if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseLeave(e);
        updateColor();
    }
}
