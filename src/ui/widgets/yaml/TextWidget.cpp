#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

#include "Vorb/graphics/FontCache.h"
#include "Vorb/ui/widgets/TextWidget.h"
#include "Vorb/ui/widgets/yaml/Parser.h"
#include "Vorb/ui/widgets/yaml/Widget.h"

bool vui::parseTextWidgetEntry(keg::ReadContext& context, vui::TextWidget* textWidget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::FontCache* fontCache) {
    if (name == "text") {
        nString text;
        if (!parseValue(*value, text)) return false;

        textWidget->setText(text);
    } else if (name == "text_scale") {
        f32v2 scale;
        if (!parseVec2(*value, scale)) return false;

        textWidget->setTextScale(scale);
    } else if (name == "text_align") {
        vg::TextAlign textAlign;
        if (!parseTextAlign(*value, textAlign)) return false;

        textWidget->setTextAlign(textAlign);
    } else if (name == "text_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        textWidget->setTextColor(color);
    } else if (name == "font") {
        FontDescriptor fontDescriptor;
        if (!parseFont(*value, fontDescriptor)) return false;

        vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

        textWidget->setFont(font);
    } else if (name == "hover_text") {
        nString text;
        if (!parseValue(*value, text)) return false;

        textWidget->setHoverText(text);
    } else if (name == "hover_text_scale") {
        f32v2 scale;
        if (!parseVec2(*value, scale)) return false;

        textWidget->setHoverTextScale(scale);
    } else if (name == "hover_text_align") {
        vg::TextAlign textAlign;
        if (!parseTextAlign(*value, textAlign)) return false;

        textWidget->setHoverTextAlign(textAlign);
    } else if (name == "hover_text_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        textWidget->setHoverTextColor(color);
    } else if (name == "hover_font") {
        FontDescriptor fontDescriptor;
        if (!parseFont(*value, fontDescriptor)) return false;

        vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

        textWidget->setHoverFont(font);
    } else {
        return vui::parseWidgetEntry(context, textWidget, name, value, widgetParser);
    }

    return true;
}
