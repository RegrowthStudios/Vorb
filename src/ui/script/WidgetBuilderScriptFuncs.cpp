#include "Vorb/stdafx.h"
#include "Vorb/ui/script/WidgetBuilderScriptFuncs.h"

vui::Button* vui::WidgetBuilderScriptFuncs::impl::makeButton(IWidget* parent, nString name, f32v4 dimensions) {
    Button* button = new Button();

    button->init(parent, name, dimensions);
    button->setSelfOwned(true);

    return button;
}

vui::CheckBox* vui::WidgetBuilderScriptFuncs::impl::makeCheckBox(IWidget* parent, nString name, f32v4 dimensions) {
    CheckBox* checkBox = new CheckBox();

    checkBox->init(parent, name, dimensions);
    checkBox->setSelfOwned(true);

    return checkBox;
}

vui::ComboBox* vui::WidgetBuilderScriptFuncs::impl::makeComboBox(IWidget* parent, nString name, f32v4 dimensions) {
    ComboBox* comboBox = new ComboBox();

    comboBox->init(parent, name, dimensions);
    comboBox->setSelfOwned(true);

    return comboBox;
}

vui::Label* vui::WidgetBuilderScriptFuncs::impl::makeLabel(IWidget* parent, nString name, f32v4 dimensions) {
    Label* label = new Label();

    label->init(parent, name, dimensions);
    label->setSelfOwned(true);

    return label;
}

vui::Panel* vui::WidgetBuilderScriptFuncs::impl::makePanel(IWidget* parent, nString name, f32v4 dimensions) {
    Panel* panel = new Panel();

    panel->init(parent, name, dimensions);
    panel->setSelfOwned(true);

    return panel;
}

vui::Slider* vui::WidgetBuilderScriptFuncs::impl::makeSlider(IWidget* parent, nString name, f32v4 dimensions) {
    Slider* slider = new Slider();

    slider->init(parent, name, dimensions);
    slider->setSelfOwned(true);

    return slider;
}

vui::WidgetList* vui::WidgetBuilderScriptFuncs::impl::makeWidgetList(IWidget* parent, nString name, f32v4 dimensions) {
    WidgetList* widgetList = new WidgetList();

    widgetList->init(parent, name, dimensions);
    widgetList->setSelfOwned(true);

    return widgetList;
}

void vui::WidgetBuilderScriptFuncs::impl::destroyWidgetTree(IWidget* toplevelWidget) {
    toplevelWidget->dispose();

    if (toplevelWidget->isSelfOwned()) delete toplevelWidget;
}
