#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/Button.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/Button.h"
#include "Vorb/ui/widgets/yaml/Parser.h"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

bool vui::parseButtonEntry(keg::ReadContext& context, vui::Button* button, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache) {
    if (name == "bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        button->setBackColor(color);
    } else if (name == "bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        button->setBackHoverColor(color);
    } else if (name == "bg_gradient") {
        color4           color1, color2;
        vg::GradientType gradType;
        if (!parseGradient(*value, color1, color2, gradType)) return false;

        button->setBackColorGrad(color1, color2, gradType);
    } else if (name == "bg_hover_gradient") {
        color4           color1, color2;
        vg::GradientType gradType;
        if (!parseGradient(*value, color1, color2, gradType)) return false;

        button->setBackHoverColorGrad(color1, color2, gradType);
    } else if (name == "bg_texture") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        nString texturePath = value->data.as<nString>();

        vg::Texture tex = textureCache->addTexture(texturePath);

        button->setTexture(tex.id);
    } else if (name == "bg_hover_texture") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        nString texturePath = value->data.as<nString>();

        vg::Texture tex = textureCache->addTexture(texturePath);

        button->setHoverTexture(tex.id);
    } else if (name == "text_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        button->setTextHoverColor(color);
    } else {
        return vui::parseTextWidgetEntry(context, button, name, value, widgetParser);
    }

    return true;
}
