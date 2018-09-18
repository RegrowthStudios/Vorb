#include "Vorb/stdafx.h"
#include "Vorb/ui/ComboBox.h"

#ifndef VORB_USING_PCH
#undef min
#endif // !VORB_USING_PCH

#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/utils.h"

vui::ComboBox::ComboBox() : Widget() {
    m_maxDropHeight = FLT_MAX;
    m_dropPanel     = Panel();
    m_mainButton    = Button();
    m_buttons       = std::vector<Button*>();
    m_items         = std::vector<nString>();
    m_dropDownStyle = DropDownStyle::DROP_DOWN_LIST;
    m_isDropped     = false;

    ValueChange.setSender(this);

    addWidget(&m_mainButton);
    addWidget(&m_dropPanel);

    m_dropPanel.setAutoScroll(true);
    m_dropPanel.setClipping({ ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN });

    m_mainButton.MouseClick += makeDelegate(*this, &ComboBox::onMainButtonClick);

    m_mainButton.setTextAlign(vg::TextAlign::LEFT);
}

vui::ComboBox::ComboBox(const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/) : ComboBox() {
    m_name = name;

    m_position = f32v2(dimensions.x, dimensions.y);
    m_size     = f32v2(dimensions.z, dimensions.w);

    m_rawDimensions.position.x = dimensions.x;
    m_rawDimensions.position.y = dimensions.y;
    m_rawDimensions.size.x     = dimensions.z;
    m_rawDimensions.size.y     = dimensions.w;

    m_flags.needsDimensionUpdate = true;
}

vui::ComboBox::ComboBox(const nString& name, const Length2& position, const Length2& size) : ComboBox() {
    m_name = name;

    m_rawDimensions.position = position;
    m_rawDimensions.size     = size;

    m_flags.needsDimensionUpdate = true;
}

vui::ComboBox::ComboBox(IWidget* parent, const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/) : ComboBox(name, dimensions) {
    parent->addWidget(this);
}

vui::ComboBox::ComboBox(IWidget* parent, const nString& name, const Length2& position, const Length2& size) : ComboBox(name, position, size) {
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

vui::Button* vui::ComboBox::addItem(const nString& item) {
    // Add item and correspodning button.
    m_items.push_back(item);
    m_buttons.push_back(new Button);

    // Get reference to button we just added.
    auto& button = m_buttons.back();

    // Add button as child of drop panel and add the text given.
    m_dropPanel.addWidget(button);
    button->setText(item);

    // If the combobox isn't currently dropped, disable the button.
    if (!m_isDropped) {
        button->disable();
    }

    // Add listener for mouse clicks on the button to handle combobox logic.
    button->MouseClick += makeDelegate(*this, &ComboBox::onSubButtonClick);

    button->setSize(m_size);
    
    m_flags.needsDrawableRecalculation = true;

    return button;
}

vui::Button* vui::ComboBox::addItemAtIndex(size_t index, const nString& item) {
    // Return false if index is out-of-range.
    if (index >= m_buttons.size()) return nullptr;

    // Add item and correspodning button.
    m_items.insert(m_items.begin() + index, item);
    m_buttons.insert(m_buttons.begin() + index, new Button);

    // Get reference to button we just added.
    auto& button = *(m_buttons.begin() + index);

    // Add button as child of drop panel and add the text given.
    m_dropPanel.addWidget(button);
    button->setText(item);

    // If the combobox isn't currently dropped, disable the button.
    if (!m_isDropped) {
        button->disable();
    }

    // Add listener for mouse clicks on the button to handle combobox logic.
    button->MouseClick += makeDelegate(*this, &ComboBox::onSubButtonClick);

    button->setSize(m_size);
    
    m_flags.needsDrawableRecalculation = true;

    return button;
}

std::vector<vui::Button*> vui::ComboBox::addItems(const std::vector <nString>& itemsToAdd) {
    std::vector<Button*> addedButtons;
    addedButtons.reserve(itemsToAdd.size());

    for (auto& item : itemsToAdd) {
        addedButtons.push_back(addItem(item));
    }

    return addedButtons;
}

bool vui::ComboBox::removeItem(const nString& item) {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (*(m_items.begin() + i) == item) {
            m_items.erase(m_items.begin() + i);
            m_buttons.erase(m_buttons.begin() + i);
            return true;
        }
    }
    return false;
}

bool vui::ComboBox::removeItem(size_t index) {
    if (index >= m_buttons.size()) return false;

    m_items.erase(m_items.begin() + index);
    m_buttons.erase(m_buttons.begin() + index);

    return true;
}

bool vui::ComboBox::selectItem(const nString& item) {
    // If this item isn't currently selected, then we search for it and set it if found.
    if (item != m_mainButton.getText()) {
        for (auto& button : m_buttons) {
            if (button->getText() != item) continue;

            m_mainButton.setText(item);

            ValueChange(item);

            return true;
        }
    }
    return false;
}

bool vui::ComboBox::selectItem(size_t index) {
    // Fail if out-of-range.
    if (index >= m_buttons.size()) return false;

    // Get text of selected item.
    const nString& selected = m_buttons[index]->getText();

    // If this item isn't currently selected, set it as selected.
    if (selected != m_mainButton.getText()) {
        m_mainButton.setText(selected);

        ValueChange(selected);
    }
    return true;
}

bool vui::ComboBox::isInDropBounds(f32 x, f32 y) const {
    return m_dropPanel.isInBounds(x, y);
}

void vui::ComboBox::setFont(const vg::SpriteFont* font) {
    m_mainButton.setFont(font);

    for (auto& button : m_buttons) {
        button->setFont(font);
    }
}

void vui::ComboBox::setMainButtonTexture(VGTexture texture) {
    m_mainButton.setTexture(texture);
}

void vui::ComboBox::setDropBoxTexture(VGTexture texture) {
    m_dropPanel.setTexture(texture);
}

void vui::ComboBox::setDropButtonTexture(VGTexture texture) {
    for (auto& button : m_buttons) {
        button->setTexture(texture);
    }
}

void vui::ComboBox::setDropButtonTexture(VGTexture texture, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setTexture(texture);
    }
}

void vui::ComboBox::setDropButtonTexture(VGTexture texture, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setTexture(texture);
}

void vui::ComboBox::setBackColor(const color4& color) {
    setMainButtonBackColor(color);
    setDropButtonBackColor(color);
}

void vui::ComboBox::setMainButtonBackColor(const color4& color) {
    m_mainButton.setBackColor(color);
}

void vui::ComboBox::setDropButtonBackColor(const color4& color) {
    for (auto& button : m_buttons) {
        button->setBackColor(color);
    }
}

void vui::ComboBox::setDropButtonBackColor(const color4& color, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setBackColor(color);
    }
}

void vui::ComboBox::setDropButtonBackColor(const color4& color, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setBackColor(color);
}

void vui::ComboBox::setBackHoverColor(const color4& color) {
    setMainButtonBackHoverColor(color);
    setDropButtonBackHoverColor(color);
}

void vui::ComboBox::setMainButtonBackHoverColor(const color4& color) {
    m_mainButton.setBackHoverColor(color);
}

void vui::ComboBox::setDropButtonBackHoverColor(const color4& color) {
    for (auto& button : m_buttons) {
        button->setBackHoverColor(color);
    }
}

void vui::ComboBox::setDropButtonBackHoverColor(const color4& color, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setBackHoverColor(color);
    }
}

void vui::ComboBox::setDropButtonBackHoverColor(const color4& color, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setBackHoverColor(color);
}

void vui::ComboBox::setTextColor(const color4& color) {
    setMainButtonTextColor(color);
    setDropButtonTextColor(color);
}

void vui::ComboBox::setMainButtonTextColor(const color4& color) {
    m_mainButton.setTextColor(color);
}

void vui::ComboBox::setDropButtonTextColor(const color4& color) {
    for (auto& button : m_buttons) {
        button->setTextColor(color);
    }
}

void vui::ComboBox::setDropButtonTextColor(const color4& color, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setTextColor(color);
    }
}

void vui::ComboBox::setDropButtonTextColor(const color4& color, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setTextColor(color);
}

void vui::ComboBox::setTextHoverColor(const color4& color) {
    setMainButtonTextHoverColor(color);
    setDropButtonTextHoverColor(color);
}

void vui::ComboBox::setMainButtonTextHoverColor(const color4& color) {
    m_mainButton.setTextHoverColor(color);
}

void vui::ComboBox::setDropButtonTextHoverColor(const color4& color) {
    for (auto& button : m_buttons) {
        button->setTextHoverColor(color);
    }
}

void vui::ComboBox::setDropButtonTextHoverColor(const color4& color, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setTextHoverColor(color);
    }
}

void vui::ComboBox::setDropButtonTextHoverColor(const color4& color, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setTextHoverColor(color);
}


void vui::ComboBox::setTextScale(const f32v2& textScale) {
    setMainButtonTextScale(textScale);
    setDropButtonTextScale(textScale);
}

void vui::ComboBox::setMainButtonTextScale(const f32v2& textScale) {
    m_mainButton.setTextScale(textScale);
}

void vui::ComboBox::setDropButtonTextScale(const f32v2& textScale) {
    for (auto& button : m_buttons) {
        button->setTextScale(textScale);
    }
}

void vui::ComboBox::setDropButtonTextScale(const f32v2& textScale, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setTextScale(textScale);
    }
}

void vui::ComboBox::setDropButtonTextScale(const f32v2& textScale, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setTextScale(textScale);
}

void vui::ComboBox::setTextAlign(vg::TextAlign align) {
    m_mainButton.setTextAlign(align);

    for (auto& button : m_buttons) {
        button->setTextAlign(align);
    }
}

void vui::ComboBox::setMaxDropHeight(f32 maxDropHeight) {
    m_maxDropHeight = maxDropHeight;

    m_flags.needsDrawableRecalculation = true;
}

void vui::ComboBox::updateDimensions(f32 dt) {
    Widget::updateDimensions(dt);

    m_dropPanel.setWidth(getWidth());

    f32 panelHeight = (f32)m_buttons.size() * getHeight();
    bool hasSlider = false;
    if (panelHeight > m_maxDropHeight) {
        panelHeight = m_maxDropHeight;
        hasSlider = true;
    }

    f32 i = 0.0f;
    for (auto& button : m_buttons) {
        if (m_isDropped) {
            button->enable();
            button->setPositionType(PositionType::STATIC_TO_PARENT);
            button->setRawPosition({ 0.0f, i * getHeight(), { DimensionType::PIXEL, DimensionType::PIXEL } });
            if (hasSlider) {
                button->setRawSize({ getWidth() - m_dropPanel.getSliderWidth(), getHeight(), { DimensionType::PIXEL, DimensionType::PIXEL } });
            } else {
                button->setRawSize({ getWidth(), getHeight(), { DimensionType::PIXEL, DimensionType::PIXEL } });
            }
        } else {
            button->disable();
            button->setRawSize({ 0.0f, 0.0f, { DimensionType::PIXEL, DimensionType::PIXEL } });
        }
        i += 1.0f;
    }

    if (m_isDropped) {
        m_dropPanel.setPosition(f32v2(0.0f, getHeight()));
        m_dropPanel.setSize(f32v2(getWidth(), panelHeight));
    } else {
        m_dropPanel.setSize(f32v2(0.0f));
    }
}

void vui::ComboBox::calculateDrawables() {
    // Empty
}

void vui::ComboBox::onMouseMove(Sender s VORB_MAYBE_UNUSED, const MouseMotionEvent& e) {
    if (!m_flags.isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_flags.isMouseIn) {
            m_flags.isMouseIn = true;
            MouseEnter(e);
        }
        MouseMove(e);
    } else if (m_flags.isMouseIn) {
        m_flags.isMouseIn = false;
        MouseLeave(e);
    }
}

void vui::ComboBox::onMouseUp(Sender s VORB_MAYBE_UNUSED, const MouseButtonEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        MouseUp(e);
        if (!m_flags.isClicking && !isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
            m_isDropped = false;
            m_flags.needsDrawableRecalculation = true;
        }
    } else if (!isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
        m_isDropped = false;
        m_flags.needsDrawableRecalculation = true;
    }
    m_flags.isClicking = false;
}

void vui::ComboBox::onSubButtonClick(Sender s VORB_MAYBE_UNUSED, const MouseButtonEvent& e VORB_MAYBE_UNUSED) {
    vui::Button* button = (vui::Button*)s;
    const nString& text = button->getText();
    if (m_mainButton.getText() != text) {
        m_mainButton.setText(text);

        ValueChange(text);
    }
}

void vui::ComboBox::onMainButtonClick(Sender s VORB_MAYBE_UNUSED, const MouseButtonEvent& e) {
    MouseClick(e);

    m_isDropped = !m_isDropped;

    m_flags.needsDrawableRecalculation = true;
}
