#include "stdafx.h"
#include "ui/ComboBox.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"
#include "utils.h"

vui::ComboBox::ComboBox() : Widget() {
    ValueChange.setSender(this);
    addWidget(&m_mainButton);
    addWidget(&m_dropPanel);
    m_dropPanel.setAutoScroll(true);
    m_mainButton.MouseClick += makeDelegate(*this, &ComboBox::onMainButtonClick);
    m_mainButton.setTextAlign(vg::TextAlign::LEFT);
}

vui::ComboBox::ComboBox(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : ComboBox() {
    m_name = name;
    setDestRect(destRect);
    m_mainButton.setDimensions(m_dimensions);
    updatePosition();
}

vui::ComboBox::ComboBox(IWidgetContainer* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : ComboBox(name, destRect) {
    parent->addWidget(this);
}

vui::ComboBox::~ComboBox() {
    // Empty
}

void vui::ComboBox::dispose() {
    Widget::dispose();
    for (size_t i = 0; i < m_buttons.size(); i++) {
        delete m_buttons[i];
    }
    std::vector<Button*>().swap(m_buttons);
}

void vui::ComboBox::updatePosition() {

    bool hasSlider = false;
    if (m_items.size() * getHeight() > m_maxDropHeight) {
        hasSlider = true;
    }

    // Buttons
    f32 i = 0;
    for (auto& b : m_buttons) {
        if (m_isDropped) {
            b->enable();
            b->setPosition(f32v2(0.0f, i * getHeight()));
            if (hasSlider) {
                b->setDimensions(f32v2(getWidth() - m_dropPanel.getSliderWidth(), getHeight()));
            } else {
                b->setDimensions(getDimensions());
            }
        } else {
            b->disable();
            b->setDimensions(f32v2(0.0f));
        }
        i += 1.0f;
    }

    // Drop list
    if (m_isDropped) {
        m_dropPanel.setPosition(f32v2(0.0f, getHeight()));
        f32v2 dims = getDimensions() * f32v2(1.0f, m_items.size());
        dims.y = glm::min(dims.y, m_maxDropHeight);
        m_dropPanel.setDimensions(dims);
    } else {
        m_dropPanel.setDimensions(f32v2(0.0f));
    }
   
    vui::Widget::updatePosition();
}

void vui::ComboBox::addItem(const nString& item) {
    m_items.push_back(item);
    m_buttons.push_back(new Button);
    auto b = m_buttons.back();
    b->addDrawables(m_renderer); // TODO(Ben): This only works consistently in LUA
    m_dropPanel.addWidget(b);
    b->setText(item);
    if (!m_isDropped) {
        b->disable();
    }
    b->MouseClick += makeDelegate(*this, &ComboBox::onSubButtonClick);
    updateDropButton(b);
    b->setDimensions(m_dimensions);
    updatePosition();
}

bool vui::ComboBox::addItemAtIndex(int index, const nString& item) {
    if (index > (int)m_items.size()) return false;
    m_items.insert(m_items.begin() + index, item);
    return true;
}

bool vui::ComboBox::removeItem(const nString& item) {
    // TODO(Ben): This is certainly wrong. It leaves button.
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
        ValueChange(m_items[index]);
    }
    return true;
}

bool vui::ComboBox::isInDropBounds(f32 x, f32 y) const {
    return m_dropPanel.isInBounds(x, y);
}

const vorb::graphics::SpriteFont* vui::ComboBox::getFont() const {
    return m_mainButton.getFont();
}

f32v2 vui::ComboBox::getTextScale() const {
    return m_mainButton.getTextScale();
}

const nString& vui::ComboBox::getItem(int index) const {
    return m_items.at(index);
}

void vui::ComboBox::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_mainButton.setDimensions(dimensions);
    updatePosition();
}

void vui::ComboBox::setFont(const vorb::graphics::SpriteFont* font) {
    m_mainButton.setFont(font);
    for (auto& b : m_buttons) b->setFont(font);
}

void vui::ComboBox::setHeight(f32 height) {
    Widget::setHeight(height);
    m_mainButton.setHeight(height);
    updatePosition();
}

void vui::ComboBox::setTexture(VGTexture texture) {
    m_mainButton.setTexture(texture);
}

void vui::ComboBox::setDropBoxTexture(VGTexture texture) {
    m_dropPanel.setTexture(texture);
}

void vui::ComboBox::setDropButtonTexture(VGTexture texture) {
    for (auto& b : m_buttons) b->setTexture(texture);
}

void vui::ComboBox::setWidth(f32 width) {
    Widget::setWidth(width);
    m_mainButton.setWidth(width);
    updatePosition();
}

void vui::ComboBox::setBackColor(const color4& color) {
    m_mainButton.setBackColor(color);
    for (auto& b : m_buttons)  b->setBackColor(color);
}

void vui::ComboBox::setBackHoverColor(const color4& color) {
    m_mainButton.setBackHoverColor(color);
    for (auto& b : m_buttons)  b->setBackHoverColor(color);
}

void vui::ComboBox::setTextColor(const color4& color) {
    m_mainButton.setTextColor(color);
    for (auto& b : m_buttons)  b->setTextColor(color);
}

void vui::ComboBox::setTextHoverColor(const color4& color) {
    m_mainButton.setTextHoverColor(color);
    for (auto& b : m_buttons) b->setTextHoverColor(color);
}

void vui::ComboBox::setTextScale(const f32v2& textScale) {
    m_mainButton.setTextScale(textScale);
    for (auto& it : m_buttons) {
        it->setTextScale(textScale);
    }
}

void vui::ComboBox::setTextAlign(vg::TextAlign align) {
    m_mainButton.setTextAlign(align);
    for (auto& it : m_buttons) {
        it->setTextAlign(align);
    }
}

void vui::ComboBox::setText(const nString& text) {
    m_mainButton.setText(text);
}

void vui::ComboBox::setMaxDropHeight(f32 maxDropHeight) {
    m_maxDropHeight = maxDropHeight;
    updatePosition();
}

void vui::ComboBox::updateDropButton(vui::Button* b) {
    b->setFont(m_mainButton.getFont());
    b->setBackColor(m_mainButton.getBackColor());
    b->setBackHoverColor(m_mainButton.getBackHoverColor());
    b->setTextColor(m_mainButton.getTextColor());
    b->setTextHoverColor(m_mainButton.getTextHoverColor());
    b->setTextAlign(m_mainButton.getTextAlign());
    b->setTextScale(m_mainButton.getTextScale());
}

void vui::ComboBox::computeClipRect(const f32v4& parentClipRect /*= f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX)*/) {
    if (m_isClippingEnabled) {
        f32v2 pos = m_position;
        f32v2 dims = m_dimensions;

        dims.y += m_dropPanel.getHeight();

        computeClipping(parentClipRect, pos, dims);
        if (dims.x < 0) dims.x = 0;
        if (dims.y < 0) dims.y = 0;
        m_clipRect = f32v4(pos.x, pos.y, dims.x, dims.y);
    } else {
        m_clipRect = parentClipRect;
    }
    computeChildClipRects();
}

void vui::ComboBox::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_isMouseIn) {
            m_isMouseIn = true;
            MouseEnter(e);
        }
        MouseMove(e);
    } else if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseLeave(e);
    }
}

void vui::ComboBox::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseUp(e);
        if (!m_isClicking && !isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
            m_isDropped = false;
            updatePosition();
        }
    } else if (!isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
        m_isDropped = false;
        updatePosition();
    }
    m_isClicking = false;
}

void vui::ComboBox::onSubButtonClick(Sender s, const MouseButtonEvent& e) {
    vui::Button* b = (vui::Button*)s;
    const nString& text = b->getText();
    if (m_mainButton.getText() != text) {
        m_mainButton.setText(text);
        ValueChange(text);
    }
}

void vui::ComboBox::onMainButtonClick(Sender s, const MouseButtonEvent& e) {
    MouseClick(e);
    m_isDropped = !m_isDropped;
    updatePosition();
}
