#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

#include "Vorb/ui/widgets/TextWidget.h"
#include "Vorb/ui/widgets/yaml/Widget.h"

bool parseTextAlign(const nString& name, OUT vg::TextAlign& textAlign) {
    if (name == "none") {
        textAlign = vg::TextAlign::NONE;
        return true;
    } else if (name == "left") {
        textAlign = vg::TextAlign::LEFT;
        return true;
    } else if (name == "top_left") {
        textAlign = vg::TextAlign::TOP_LEFT;
        return true;
    } else if (name == "top") {
        textAlign = vg::TextAlign::TOP;
        return true;
    } else if (name == "top_right") {
        textAlign = vg::TextAlign::TOP_RIGHT;
        return true;
    } else if (name == "right") {
        textAlign = vg::TextAlign::RIGHT;
        return true;
    } else if (name == "bottom_right") {
        textAlign = vg::TextAlign::BOTTOM_RIGHT;
        return true;
    } else if (name == "bottom") {
        textAlign = vg::TextAlign::BOTTOM;
        return true;
    } else if (name == "bottom_left") {
        textAlign = vg::TextAlign::BOTTOM_LEFT;
        return true;
    } else if (name == "center") {
        textAlign = vg::TextAlign::CENTER;
        return true;
    }
    return false;
}

bool vui::parseTextWidgetEntry(keg::ReadContext& context, vui::TextWidget* textWidget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser) {
    if (name == "text") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        textWidget->setText(value->data[0].as<nString>());
    } else if (name == "text_scale") {
        if (keg::getType(value) != keg::NodeType::SEQUENCE
                || value->data.size() != 2) return false;
        
        f32v2 scale = f32v2(value->data[0].as<f32>(), value->data[1].as<f32>());

        textWidget->setTextScale(scale);
    } else if (name == "text_align") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        vg::TextAlign textAlign;
        if (!parseTextAlign(value->data[0].as<nString>(), textAlign)) return false;

        textWidget->setTextAlign(textAlign);
    } else if (name == "text_color") {
        if (keg::getType(value) != keg::NodeType::SEQUENCE
                || value->data.size() != 4) return false;

        color4 color;
        color.r = value->data[0].as<ui8>();
        color.g = value->data[1].as<ui8>();
        color.b = value->data[2].as<ui8>();
        color.a = value->data[3].as<ui8>();

        textWidget->setTextColor(color);
    } else if (name == "font") {
        // TODO(Matthew): Implement. This should likely have a cache mechanism.
    }

    return vui::parseWidgetEntry(context, label, name, value, widgetParser);
}
