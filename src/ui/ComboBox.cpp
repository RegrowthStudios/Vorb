#include "stdafx.h"
#include "ui/ComboBox.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"

vui::ComboBox::ComboBox() : Widget() {
    ValueChange.setSender(this);
    addWidget(&m_mainButton);
    m_mainButton.MouseClick += makeDelegate(*this, &ComboBox::onMainButtonClick);
    updateColor();
}

vui::ComboBox::ComboBox(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : ComboBox() {
    m_name = name;
    setDestRect(destRect);
    m_mainButton.setDimensions(m_dimensions);
    updateTextPosition();
}

vui::ComboBox::ComboBox(Widget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : ComboBox(name, destRect) {
    parent->addWidget(this);
}

vui::ComboBox::~ComboBox() {
    // Empty
}

void vui::ComboBox::addDrawables(UIRenderer* renderer) {
    removeDrawables(renderer);

    m_defaultFont = renderer->getDefaultFont();
    // Make copies
    m_drawnRect = m_drawableRect;
    m_drawableDropList = m_drawableDropList;

    // Add the main button
    m_mainButton.removeDrawables(renderer);
    m_mainButton.addDrawables(renderer);

    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &ComboBox::refreshDrawables));

    // Add the drop list
    renderer->add(this,
                  makeDelegate(m_drawnDropList, &DrawableRect::draw),
                  makeDelegate(*this, &ComboBox::refreshDrawables));

    // Add the drop down texts TODO(Ben): Inefficient
    for (auto& it : m_buttons) {
        it.removeDrawables(renderer);
        it.addDrawables(renderer);
    }
}

void vui::ComboBox::removeDrawables(UIRenderer* renderer) {
    renderer->remove(this);
}

void vui::ComboBox::addItem(const nString& item) {
    m_items.push_back(item);
    m_buttons.emplace_back();
    auto& b = m_buttons.back();
    addWidget(&b);
    b.setText(item);
    if (!m_isDropped) {
        b.disable();
    }
    b.MouseClick += makeDelegate(*this, &ComboBox::onSubButtonClick);
    updateDropButton(b);
    b.setDimensions(m_dimensions);
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
    if (index > (int)m_items.size()) return false;
    if (m_items[index] != m_mainButton.getText()) {
        m_mainButton.setText(m_items[index]);
        refreshDrawables();
        ValueChange(m_items[index]);
    }
    return true;
}

bool vui::ComboBox::isInDropBounds(f32 x, f32 y) {
    const f32v2& pos = m_drawableDropList.getPosition();
    const f32v2& dims = m_drawableDropList.getDimensions();
    return (x >= pos.x && x <= pos.x + dims.x &&
            y >= pos.y && y <= pos.y + dims.y);
}

const vorb::graphics::SpriteFont* vui::ComboBox::getFont() const {
    return m_mainButton.getFont();
}

f32v2 vui::ComboBox::getTextScale() const {
    return m_mainButton.getTextScale();
}

void vui::ComboBox::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
    updateTextPosition();
}

void vui::ComboBox::setFont(const vorb::graphics::SpriteFont* font) {
    m_mainButton.setFont(font);
    for (auto& b : m_buttons) b.setFont(font);
    refreshDrawables();
}

void vui::ComboBox::setHeight(f32 height) {
    Widget::setHeight(height);
    m_mainButton.setHeight(height);
    updateTextPosition();
}

void vui::ComboBox::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_mainButton.setPosition(m_position);
    updateTextPosition();
}

void vui::ComboBox::setTexture(VGTexture texture) {
    m_mainButton.setTexture(texture);
    refreshDrawables();
}

void vui::ComboBox::setDropBoxTexture(VGTexture texture) {
    m_drawableDropList.setTexture(texture);
    refreshDrawables();
}

void vui::ComboBox::setDropButtonTexture(VGTexture texture) {
    for (auto& b : m_buttons) b.setTexture(texture);
}

void vui::ComboBox::setWidth(f32 width) {
    Widget::setWidth(width);
    m_mainButton.setWidth(width);
    updateTextPosition();
}

void vui::ComboBox::setX(f32 x) {
    Widget::setX(x);
    m_mainButton.setX(m_position.x);
    updateTextPosition();
}

void vui::ComboBox::setY(f32 y) {
    Widget::setY(y);
    m_mainButton.setX(m_position.y);
    updateTextPosition();
}

void vui::ComboBox::setBackColor(const color4& color) {
    m_backColor = color;
    m_mainButton.setBackColor(color);
    for (auto& b : m_buttons)  b.setBackColor(color);
    updateColor();
}

void vui::ComboBox::setBackHoverColor(const color4& color) {
    m_backHoverColor = color;
    m_mainButton.setBackHoverColor(color);
    for (auto& b : m_buttons)  b.setBackHoverColor(color);
    updateColor();
}

void vui::ComboBox::setTextColor(const color4& color) {
    m_textColor = color;
    m_mainButton.setTextColor(color);
    for (auto& b : m_buttons)  b.setTextColor(color);
    updateColor();
}

void vui::ComboBox::setTextHoverColor(const color4& color) {
    m_textHoverColor = color;
    m_mainButton.setTextHoverColor(color);
    for (auto& b : m_buttons) b.setTextHoverColor(color);
    updateColor();
}

void vui::ComboBox::setTextScale(const f32v2& textScale) {
    m_mainButton.setTextScale(textScale);
    for (auto& it : m_buttons) {
        it.setTextScale(textScale);
    }
    refreshDrawables();
}

void vui::ComboBox::updateDropButton(vui::Button& b) {
    b.setFont(m_mainButton.getFont());
    b.setBackColor(m_backColor);
    b.setBackHoverColor(m_backHoverColor);
    b.setTextColor(m_textColor);
    b.setTextHoverColor(m_textHoverColor);
}

void vui::ComboBox::updateColor() {
    if (m_isMouseIn) {
        m_mainButton.setBackColor(m_backHoverColor);
        m_mainButton.setTextColor(m_textHoverColor);
        m_drawableRect.setColor(m_backHoverColor);
    } else {
        m_mainButton.setBackColor(m_backColor);
        m_mainButton.setTextColor(m_textColor);
        m_drawableRect.setColor(m_backColor);
    }
    refreshDrawables();
}

void vui::ComboBox::updateTextPosition() {
    ui32 i = 1;
    for (auto& b : m_buttons) {
        if (m_isDropped) {
            b.enable();
            b.setPosition(f32v2(0.0f, i * getHeight()));
            b.setDimensions(getDimensions());
        } else {
            b.disable();
            b.setDimensions(f32v2(0.0f));
        }
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

    m_drawnRect = m_drawableRect;
}

void vui::ComboBox::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
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

void vui::ComboBox::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseUp(e);
        if (!m_isClicking && !isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
            m_isDropped = false;
            updateTextPosition();
        }
    } else if (!isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
        m_isDropped = false;
        updateTextPosition();
    }
    m_isClicking = false;
}

void vui::ComboBox::onSubButtonClick(Sender s, const MouseButtonEvent& e) {
    vui::Button* b = (vui::Button*)s;
    const nString& text = b->getText();
    if (m_mainButton.getText() != text) {
        m_mainButton.setText(text);
        refreshDrawables();
        ValueChange(text);
    }
}

void vui::ComboBox::onMainButtonClick(Sender s, const MouseButtonEvent& e) {
    MouseClick(e);
    m_isDropped = !m_isDropped;
    updateTextPosition();
}
