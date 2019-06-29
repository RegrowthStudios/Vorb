#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

#include "Vorb/ui/widgets/TextWidget.h"
#include "Vorb/ui/widgets/yaml/helper.hpp"
#include "Vorb/ui/widgets/yaml/Widget.h"

bool vui::parseTextWidgetEntry(keg::ReadContext& context, vui::TextWidget* textWidget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser) {
    if (name == "text") {
        nString text;
        if (!parseString(value, text)) return false;

        textWidget->setText(text);
    } else if (name == "text_scale") {
        f32v2 scale;
        if (!parseVec2(value, scale)) return false;

        textWidget->setTextScale(scale);
    } else if (name == "text_align") {
        vg::TextAlign textAlign;
        if (!parseTextAlign(value, textAlign)) return false;

        textWidget->setTextAlign(textAlign);
    } else if (name == "text_color") {
        color4 color;
        if (!parseColor(value, color)) return false;

        textWidget->setTextColor(color);
    } else if (name == "font") {
        // TODO(Matthew): Implement. This should likely have a cache mechanism.
    }

    return vui::parseWidgetEntry(context, textWidget, name, value, widgetParser);
}
