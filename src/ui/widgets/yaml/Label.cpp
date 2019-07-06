#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/Label.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/Label.h"
#include "Vorb/ui/widgets/yaml/helper.hpp"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

bool vui::parseLabelEntry(keg::ReadContext& context, vui::Label* label, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache) {
    if (name == "bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        label->setLabelColor(color);
    } else if (name == "bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        label->setLabelHoverColor(color);
    } else if (name == "bg_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        label->setLabelTexture(tex.id);
    } else if (name == "bg_hover_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        label->setLabelHoverTexture(tex.id);
    } else if (name == "text_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        label->setTextHoverColor(color);
    } else {
        return vui::parseTextWidgetEntry(context, label, name, value, widgetParser);
    }

    return true;
}
