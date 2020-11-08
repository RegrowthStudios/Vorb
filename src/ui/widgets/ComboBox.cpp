#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/ComboBox.h"

#ifndef VORB_USING_PCH
#undef min
#endif // !VORB_USING_PCH

#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/utils.h"

vui::ComboBox::ComboBox() :
    Widget(),
    m_maxDropHeight(FLT_MAX),
    // m_dropDownStyle(DropDownStyle::DROP_DOWN_LIST),
    m_isDropped(false) {
    m_dropPanel.setAutoScroll(true);
    m_dropPanel.setClipping(Clipping{ ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN });

    m_mainButton.setTextAlign(vg::TextAlign::LEFT);
}

vui::ComboBox::ComboBox(const ComboBox& widget) :
    Widget(widget),
    m_maxDropHeight(widget.m_maxDropHeight),
    m_dropPanel(widget.m_dropPanel),
    m_mainButton(widget.m_mainButton),
    m_buttons(std::vector<Button*>()),
    m_items(std::vector<nString>()),
    m_isDropped(widget.m_isDropped) {
    ValueChange.setSender(this);
}

vui::ComboBox::ComboBox(ComboBox&& widget) :
    Widget(std::forward<ComboBox>(widget)),
    ValueChange(std::move(widget.ValueChange)),
    m_maxDropHeight(widget.m_maxDropHeight),
    m_dropPanel(std::move(widget.m_dropPanel)),
    m_mainButton(std::move(widget.m_mainButton)),
    m_buttons(std::move(widget.m_buttons)),
    m_items(std::move(widget.m_items)),
    m_isDropped(widget.m_isDropped) {
    std::vector<Button*>().swap(m_buttons);
    std::vector<nString>().swap(m_items);

    ValueChange.setSender(this);
}

vui::ComboBox& vui::ComboBox::operator=(const ComboBox& rhs) {
    Widget::operator=(rhs);

    m_maxDropHeight = rhs.m_maxDropHeight;
    m_dropPanel     = rhs.m_dropPanel;
    m_mainButton    = rhs.m_mainButton;
    m_buttons       = std::vector<Button*>();
    m_items         = std::vector<nString>();
    m_isDropped     = rhs.m_isDropped;

    ValueChange.setSender(this);

    return *this;
}

vui::ComboBox& vui::ComboBox::operator=(ComboBox&& rhs) {
    Widget::operator=(std::forward<ComboBox>(rhs));

    ValueChange     = std::move(rhs.ValueChange);
    m_maxDropHeight = rhs.m_maxDropHeight;
    m_dropPanel     = std::move(rhs.m_dropPanel);
    m_mainButton    = std::move(rhs.m_mainButton);
    m_buttons       = std::move(rhs.m_buttons);
    m_items         = std::move(rhs.m_items);
    m_isDropped     = rhs.m_isDropped;

    ValueChange.setSender(this);

    return *this;
}

void vui::ComboBox::initBase() {
    Widget::initBase();

    ValueChange.setSender(this);

    m_dropPanel.init(this, getName() + "_drop_panel");
    m_mainButton.init(this, getName() + "_main_button", f32v4(0.0f, 0.0f, m_size.x, m_size.y));
}

void vui::ComboBox::dispose(bool thisOnly /*= false*/) {
    Widget::dispose(thisOnly);

    for (size_t i = 0; i < m_buttons.size(); i++) {
        delete m_buttons[i];
    }

    std::vector<Button*>().swap(m_buttons);
}

void vui::ComboBox::enable() {
    if (!m_flags.isEnabled) {
        m_mainButton.MouseClick += makeDelegate(this, &ComboBox::onMainButtonClick);
    }

    Widget::enable();

    if (!m_isDropped) {
        m_dropPanel.disable();
        for (auto& button : m_buttons) {
            button->disable();
        }
    }
}

void vui::ComboBox::disable(bool thisOnly /*= false*/) {
    if (m_flags.isEnabled) {
        m_mainButton.MouseClick -= makeDelegate(this, &ComboBox::onMainButtonClick);
    }

    Widget::disable(thisOnly);
}

vui::Button* vui::ComboBox::addItem(const nString& item) {
    // Add item and correspodning button.
    m_items.push_back(item);
    m_buttons.push_back(new Button());

    // Get reference to button we just added.
    auto& button = m_buttons.back();

    // Initialise button and set parent.
    button->init(&m_dropPanel, item, f32v4(0.0f, 0.0f, 100.0f, 30.0f));

    // Set the text given.
    button->setText(item);

    // If the combobox is currently dropped, enable the button.
    if (m_isDropped) {
        button->enable();
    }

    // Add listener for mouse clicks on the button to handle combobox logic.
    button->MouseClick += makeDelegate(this, &ComboBox::onSubButtonClick);

    button->setSize(m_size);
    
    m_flags.needsDrawableRecalculation = true;

    return button;
}

vui::Button* vui::ComboBox::addItemAtIndex(size_t index, const nString& item) {
    // Return false if index is out-of-range.
    if (index >= m_buttons.size()) return nullptr;

    // Add item and correspodning button.
    m_items.insert(m_items.begin() + index, item);
    m_buttons.insert(m_buttons.begin() + index, new Button());

    // Get reference to button we just added.
    auto& button = *(m_buttons.begin() + index);

    // Initialise button and set parent.
    button->init(&m_dropPanel, item, f32v4(0.0f, 0.0f, 100.0f, 30.0f));

    // Set the text given.
    button->setText(item);

    // If the combobox is currently dropped, enable the button.
    if (m_isDropped) {
        button->enable();
    }

    // Add listener for mouse clicks on the button to handle combobox logic.
    button->MouseClick += makeDelegate(this, &ComboBox::onSubButtonClick);

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

const vorb::graphics::SpriteFont* vui::ComboBox::getDropButtonFont(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getFont();
    }
    return nullptr;
}

const vorb::graphics::SpriteFont* vui::ComboBox::getDropButtonFont(size_t index) const {
    if (index >= m_buttons.size()) return nullptr;

    return m_buttons[index]->getFont();
}

const vorb::graphics::SpriteFont* vui::ComboBox::getDropButtonHoverFont(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getHoverFont();
    }
    return nullptr;
}

const vorb::graphics::SpriteFont* vui::ComboBox::getDropButtonHoverFont(size_t index) const {
    if (index >= m_buttons.size()) return nullptr;

    return m_buttons[index]->getHoverFont();
}

VGTexture vui::ComboBox::getDropButtonTexture(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getTexture();
    }
    return 0;
}

VGTexture vui::ComboBox::getDropButtonTexture(size_t index) const {
    if (index >= m_buttons.size()) return 0;

    return m_buttons[index]->getTexture();
}

VGTexture vui::ComboBox::getDropButtonHoverTexture(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getHoverTexture();
    }
    return 0;
}

VGTexture vui::ComboBox::getDropButtonHoverTexture(size_t index) const {
    if (index >= m_buttons.size()) return 0;

    return m_buttons[index]->getHoverTexture();
}

color4 vui::ComboBox::getDropButtonBackColor(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getBackColor();
    }
    return color4(0.0f, 0.0f, 0.0f, 0.0f);
}

color4 vui::ComboBox::getDropButtonBackColor(size_t index) const {
    if (index >= m_buttons.size()) return color4(0.0f, 0.0f, 0.0f, 0.0f);

    return m_buttons[index]->getBackColor();
}

color4 vui::ComboBox::getDropButtonBackHoverColor(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getBackHoverColor();
    }
    return color4(0.0f, 0.0f, 0.0f, 0.0f);
}

color4 vui::ComboBox::getDropButtonBackHoverColor(size_t index) const {
    if (index >= m_buttons.size()) return color4(0.0f, 0.0f, 0.0f, 0.0f);

    return m_buttons[index]->getBackHoverColor();
}

color4 vui::ComboBox::getDropButtonTextColor(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getTextColor();
    }
    return color4(0.0f, 0.0f, 0.0f, 0.0f);
}

color4 vui::ComboBox::getDropButtonTextColor(size_t index) const {
    if (index >= m_buttons.size()) return color4(0.0f, 0.0f, 0.0f, 0.0f);

    return m_buttons[index]->getTextColor();
}

color4 vui::ComboBox::getDropButtonHoverTextColor(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getHoverTextColor();
    }
    return color4(0.0f, 0.0f, 0.0f, 0.0f);
}

color4 vui::ComboBox::getDropButtonHoverTextColor(size_t index) const {
    if (index >= m_buttons.size()) return color4(0.0f, 0.0f, 0.0f, 0.0f);

    return m_buttons[index]->getHoverTextColor();
}

f32v2 vui::ComboBox::getDropButtonTextScale(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getTextScale();
    }
    return f32v2(0.0f);
}

f32v2 vui::ComboBox::getDropButtonTextScale(size_t index) const {
    if (index >= m_buttons.size()) return f32v2(0.0f);

    return m_buttons[index]->getTextScale();
}

f32v2 vui::ComboBox::getDropButtonHoverTextScale(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getHoverTextScale();
    }
    return f32v2(0.0f);
}

f32v2 vui::ComboBox::getDropButtonHoverTextScale(size_t index) const {
    if (index >= m_buttons.size()) return f32v2(0.0f);

    return m_buttons[index]->getHoverTextScale();
}

vg::TextAlign vui::ComboBox::getDropButtonTextAlign(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getTextAlign();
    }
    return vg::TextAlign::NONE;
}

vg::TextAlign vui::ComboBox::getDropButtonTextAlign(size_t index) const {
    if (index >= m_buttons.size()) return vg::TextAlign::NONE;

    return m_buttons[index]->getTextAlign();
}

vg::TextAlign vui::ComboBox::getDropButtonHoverTextAlign(const nString& item) const {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        return button->getHoverTextAlign();
    }
    return vg::TextAlign::NONE;
}

vg::TextAlign vui::ComboBox::getDropButtonHoverTextAlign(size_t index) const {
    if (index >= m_buttons.size()) return vg::TextAlign::NONE;

    return m_buttons[index]->getHoverTextAlign();
}

void vui::ComboBox::setFont(const vg::SpriteFont* font) {
    setMainButtonFont(font);
    setDropButtonFont(font);
}

void vui::ComboBox::setMainButtonFont(const vg::SpriteFont* font) {
    m_mainButton.setFont(font);
}

void vui::ComboBox::setDropButtonFont(const vg::SpriteFont* font) {
    for (auto& button : m_buttons) {
        button->setFont(font);
    }
}

void vui::ComboBox::setDropButtonFont(const vg::SpriteFont* font, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setFont(font);
    }
}

void vui::ComboBox::setDropButtonFont(const vg::SpriteFont* font, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setFont(font);
}

void vui::ComboBox::setDropButtonFont(const vg::SpriteFont* font, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setFont(font);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setHoverFont(const vg::SpriteFont* font) {
    setMainButtonHoverFont(font);
    setDropButtonHoverFont(font);
}

void vui::ComboBox::setMainButtonHoverFont(const vg::SpriteFont* font) {
    m_mainButton.setHoverFont(font);
}

void vui::ComboBox::setDropButtonHoverFont(const vg::SpriteFont* font) {
    for (auto& button : m_buttons) {
        button->setHoverFont(font);
    }
}

void vui::ComboBox::setDropButtonHoverFont(const vg::SpriteFont* font, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setHoverFont(font);
    }
}

void vui::ComboBox::setDropButtonHoverFont(const vg::SpriteFont* font, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setHoverFont(font);
}

void vui::ComboBox::setDropButtonHoverFont(const vg::SpriteFont* font, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setHoverFont(font);
            return;
        }
        ++i;
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

void vui::ComboBox::setDropButtonTexture(VGTexture texture, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setTexture(texture);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setMainButtonHoverTexture(VGTexture texture) {
    m_mainButton.setHoverTexture(texture);
}

void vui::ComboBox::setDropBoxHoverTexture(VGTexture texture) {
    m_dropPanel.setHoverTexture(texture);
}

void vui::ComboBox::setDropButtonHoverTexture(VGTexture texture) {
    for (auto& button : m_buttons) {
        button->setHoverTexture(texture);
    }
}

void vui::ComboBox::setDropButtonHoverTexture(VGTexture texture, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setHoverTexture(texture);
    }
}

void vui::ComboBox::setDropButtonHoverTexture(VGTexture texture, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setHoverTexture(texture);
}

void vui::ComboBox::setDropButtonHoverTexture(VGTexture texture, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setHoverTexture(texture);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setBackColor(const color4& color) {
    setMainButtonBackColor(color);
    setDropButtonBackColor(color);
}

void vui::ComboBox::setMainButtonBackColor(const color4& color) {
    m_mainButton.setBackColor(color);
}

void vui::ComboBox::setDropBoxBackColor(const color4& color) {
    m_dropPanel.setColor(color);
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

void vui::ComboBox::setDropButtonBackColor(const color4& color, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setBackColor(color);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setBackHoverColor(const color4& color) {
    setMainButtonBackHoverColor(color);
    setDropButtonBackHoverColor(color);
}

void vui::ComboBox::setMainButtonBackHoverColor(const color4& color) {
    m_mainButton.setBackHoverColor(color);
}

void vui::ComboBox::setDropBoxBackHoverColor(const color4& color) {
    m_dropPanel.setHoverColor(color);
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

void vui::ComboBox::setDropButtonBackHoverColor(const color4& color, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setBackHoverColor(color);
            return;
        }
        ++i;
    }
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

void vui::ComboBox::setDropButtonTextColor(const color4& color, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setTextColor(color);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setHoverTextColor(const color4& color) {
    setMainButtonHoverTextColor(color);
    setDropButtonHoverTextColor(color);
}

void vui::ComboBox::setMainButtonHoverTextColor(const color4& color) {
    m_mainButton.setHoverTextColor(color);
}

void vui::ComboBox::setDropButtonHoverTextColor(const color4& color) {
    for (auto& button : m_buttons) {
        button->setHoverTextColor(color);
    }
}

void vui::ComboBox::setDropButtonHoverTextColor(const color4& color, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setHoverTextColor(color);
    }
}

void vui::ComboBox::setDropButtonHoverTextColor(const color4& color, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setHoverTextColor(color);
}

void vui::ComboBox::setDropButtonHoverTextColor(const color4& color, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setHoverTextColor(color);
            return;
        }
        ++i;
    }
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

void vui::ComboBox::setDropButtonTextScale(const f32v2& textScale, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setTextScale(textScale);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setHoverTextScale(const f32v2& textScale) {
    setMainButtonHoverTextScale(textScale);
    setDropButtonHoverTextScale(textScale);
}

void vui::ComboBox::setMainButtonHoverTextScale(const f32v2& textScale) {
    m_mainButton.setHoverTextScale(textScale);
}

void vui::ComboBox::setDropButtonHoverTextScale(const f32v2& textScale) {
    for (auto& button : m_buttons) {
        button->setHoverTextScale(textScale);
    }
}

void vui::ComboBox::setDropButtonHoverTextScale(const f32v2& textScale, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setHoverTextScale(textScale);
    }
}

void vui::ComboBox::setDropButtonHoverTextScale(const f32v2& textScale, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setHoverTextScale(textScale);
}

void vui::ComboBox::setDropButtonHoverTextScale(const f32v2& textScale, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setHoverTextScale(textScale);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setTextAlign(vg::TextAlign align) {
    setMainButtonTextAlign(align);
    setDropButtonTextAlign(align);
}

void vui::ComboBox::setMainButtonTextAlign(vg::TextAlign align) {
    m_mainButton.setTextAlign(align);
}

void vui::ComboBox::setDropButtonTextAlign(vg::TextAlign align) {
    for (auto& button : m_buttons) {
        button->setTextAlign(align);
    }
}

void vui::ComboBox::setDropButtonTextAlign(vg::TextAlign align, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setTextAlign(align);
    }
}

void vui::ComboBox::setDropButtonTextAlign(vg::TextAlign align, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setTextAlign(align);
}

void vui::ComboBox::setDropButtonTextAlign(vg::TextAlign align, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setTextAlign(align);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setHoverTextAlign(vg::TextAlign align) {
    setMainButtonHoverTextAlign(align);
    setDropButtonHoverTextAlign(align);
}

void vui::ComboBox::setMainButtonHoverTextAlign(vg::TextAlign align) {
    m_mainButton.setHoverTextAlign(align);
}

void vui::ComboBox::setDropButtonHoverTextAlign(vg::TextAlign align) {
    for (auto& button : m_buttons) {
        button->setHoverTextAlign(align);
    }
}

void vui::ComboBox::setDropButtonHoverTextAlign(vg::TextAlign align, const nString& item) {
    for (auto& button : m_buttons) {
        if (button->getText() != item) continue;

        button->setHoverTextAlign(align);
    }
}

void vui::ComboBox::setDropButtonHoverTextAlign(vg::TextAlign align, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setHoverTextAlign(align);
}

void vui::ComboBox::setDropButtonHoverTextAlign(vg::TextAlign align, DropButtonComparator comparator) {
    size_t i = 0;
    for (auto& button : m_buttons) {
        if (comparator(i, button)) {
            button->setHoverTextAlign(align);
            return;
        }
        ++i;
    }
}

void vui::ComboBox::setMainButtonText(const nString& text) {
    m_mainButton.setText(text);
}

void vui::ComboBox::setDropButtonText(const nString& text) {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i]->setText(text);
        m_items[i] = text;
    }
}

void vui::ComboBox::setDropButtonText(const nString& text, const nString& item) {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i]->getText() != item) continue;

        m_buttons[i]->setText(text);
        m_items[i] = text;
    }
}

void vui::ComboBox::setDropButtonText(const nString& text, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setText(text);
    m_items[index] = text;
}

void vui::ComboBox::setDropButtonText(const nString& text, DropButtonComparator comparator) {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (comparator(i, m_buttons[i])) {
            m_buttons[i]->setText(text);
            m_items[i] = text;
        }
    }
}

void vui::ComboBox::setMainButtonHoverText(const nString& text) {
    m_mainButton.setHoverText(text);
}

void vui::ComboBox::setDropButtonHoverText(const nString& text) {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i]->setHoverText(text);
        m_items[i] = text;
    }
}

void vui::ComboBox::setDropButtonHoverText(const nString& text, const nString& item) {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i]->getText() != item) continue;

        m_buttons[i]->setHoverText(text);
        m_items[i] = text;
    }
}

void vui::ComboBox::setDropButtonHoverText(const nString& text, size_t index) {
    if (index >= m_buttons.size()) return;

    m_buttons[index]->setHoverText(text);
    m_items[index] = text;
}

void vui::ComboBox::setDropButtonHoverText(const nString& text, DropButtonComparator comparator) {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (comparator(i, m_buttons[i])) {
            m_buttons[i]->setHoverText(text);
            m_items[i] = text;
        }
    }
}

void vui::ComboBox::setMaxDropHeight(f32 maxDropHeight) {
    m_maxDropHeight = maxDropHeight;

    m_flags.needsDrawableRecalculation = true;
}

void vui::ComboBox::updateDimensions(f32 dt) {
    f32v2 oldSize = getSize();
    f32v2 oldPos  = getPosition();

    Widget::updateDimensions(dt);

    f32v2 newSize = getSize();
    if (oldSize != newSize) {
        m_mainButton.setSize(newSize);
    }

    f32 panelHeight = (f32)m_buttons.size() * getHeight();
    bool hasSlider = false;
    if (panelHeight > m_maxDropHeight) {
        panelHeight = m_maxDropHeight;
        hasSlider = true;
    }

    if (m_isDropped) {
        if (!m_dropPanel.isEnabled() || oldSize != newSize || oldPos != getPosition()) {
            // We need to set the drop panel size explicitly fully here as buttons depend on it.
            m_dropPanel.enable();
            m_dropPanel.setPositionType(PositionType::STATIC_TO_PARENT);
            m_dropPanel.setPosition(f32v2(0.0f, newSize.y));
            m_dropPanel.IWidget::setPosition(getPosition() + f32v2(0.0f, newSize.y));
            m_dropPanel.setSize(f32v2(newSize.x, panelHeight));
            m_dropPanel.IWidget::setSize(f32v2(newSize.x, panelHeight));

            f32 i = 0.0f;
            for (auto& button : m_buttons) {
                button->setPositionType(PositionType::STATIC_TO_PARENT);
                button->setPosition(f32v2(0.0f, i * getHeight()));
                if (hasSlider) {
                    button->setSize(f32v2(getWidth() - m_dropPanel.getSliderWidth(), getHeight()));
                } else {
                    button->setSize(f32v2(getWidth(), getHeight()));
                }
                // Update buttons before letting panel update as it needs to know their final properties to determine 
                // slider properties.
                button->update(dt);
                i += 1.0f;
            }
        }
    } else {
        m_dropPanel.disable();
        for (auto& button : m_buttons) {
            button->disable();
        }
    }
}

void vui::ComboBox::calculateDrawables() {
    // Empty
}

void vui::ComboBox::onMouseMove(Sender, const MouseMotionEvent& e) {
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

void vui::ComboBox::onMouseUp(Sender, const MouseButtonEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        MouseUp(e);
        if (m_flags.isClicking) {
            MouseClick(e);
        } else if (!isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
            m_isDropped = false;
            m_flags.needsDimensionUpdate = true;
        }
    } else if (!m_dropPanel.isClicking() && !isInDropBounds((f32)e.x, (f32)e.y) && m_isDropped) {
        m_isDropped = false;
        m_flags.needsDimensionUpdate = true;
    }
    m_flags.isClicking = false;
}

void vui::ComboBox::onSubButtonClick(Sender s, const MouseButtonEvent&) {
    vui::Button* button = (vui::Button*)s;
    const nString& text = button->getText();
    if (m_mainButton.getText() != text) {
        m_mainButton.setText(text);

        ValueChange(text);
    }
}

void vui::ComboBox::onMainButtonClick(Sender, const MouseButtonEvent& e) {
    MouseClick(e);

    m_isDropped = !m_isDropped;

    m_flags.needsDimensionUpdate = true;
}
