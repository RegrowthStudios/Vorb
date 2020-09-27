#include "Vorb/stdafx.h"
#include "Vorb/ui/script/WidgetBuilderScriptFuncs.h"

vui::Button* vui::WidgetBuilderScriptFuncs::impl::makeButton(IWidget* parent, nString name, f32v4 dimensions) {
    Button* button = new Button();

    button->init(parent, name, dimensions);

    return button;
}

vui::CheckBox* vui::WidgetBuilderScriptFuncs::impl::makeCheckBox(IWidget* parent, nString name, f32v4 dimensions) {
    CheckBox* checkBox = new CheckBox();

    checkBox->init(parent, name, dimensions);

    return checkBox;
}

vui::ComboBox* vui::WidgetBuilderScriptFuncs::impl::makeComboBox(IWidget* parent, nString name, f32v4 dimensions) {
    ComboBox* comboBox = new ComboBox();

    comboBox->init(parent, name, dimensions);

    return comboBox;
}

vui::Label* vui::WidgetBuilderScriptFuncs::impl::makeLabel(IWidget* parent, nString name, f32v4 dimensions) {
    Label* label = new Label();

    label->init(parent, name, dimensions);

    return label;
}

vui::Panel* vui::WidgetBuilderScriptFuncs::impl::makePanel(IWidget* parent, nString name, f32v4 dimensions) {
    Panel* panel = new Panel();

    panel->init(parent, name, dimensions);

    return panel;
}

vui::Slider* vui::WidgetBuilderScriptFuncs::impl::makeSlider(IWidget* parent, nString name, f32v4 dimensions) {
    Slider* slider = new Slider();

    slider->init(parent, name, dimensions);

    return slider;
}

vui::WidgetList* vui::WidgetBuilderScriptFuncs::impl::makeWidgetList(IWidget* parent, nString name, f32v4 dimensions) {
    WidgetList* widgetList = new WidgetList();

    widgetList->init(parent, name, dimensions);

    return widgetList;
}

void vui::WidgetBuilderScriptFuncs::impl::destroyWidgetTree(IWidget* toplevelWidget, IWidgets& widgets) {
    // Function to recursively delete children we are responsible for, and dispose the rest.
    Delegate<void, IWidget*> deleteChildren = makeFunctor([&](IWidget* toplevelWidget) {
        const IWidgets& children = toplevelWidget->getWidgets();

        for (IWidget* child : children) {
            deleteChildren(child);

            auto it = std::find(widgets.begin(), widgets.end(), child);
            if (it != widgets.end()) {
                widgets.erase(it);
                delete child;
            } else {
                child->dispose(true);
            }
        }
    });

    // Kick off recursive deletion/disposal.
    deleteChildren(toplevelWidget);

    // If we manage this widget, clear up memory.
    auto it = std::find(widgets.begin(), widgets.end(), toplevelWidget);
    if (it != widgets.end()) {
        // We're about to delete the top level widget, so want to make sure the disposal, as far as the parent is concerned, goes well.
        toplevelWidget->getParent()->markChildAsDeleted(toplevelWidget);

        widgets.erase(it);
        delete toplevelWidget;
    } else {
        // We haven't deleted the top level widget, but we do want it removed from the parent.
        toplevelWidget->getParent()->markChildForRemoval(toplevelWidget);

        toplevelWidget->dispose(true);
    }
}
