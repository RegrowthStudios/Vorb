#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/Label.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/Label.h"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

bool vui::parseLabelEntry(keg::ReadContext& context, vui::Label* label, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache) {
    if (name == "color") {
        
    } else if (name == "hover_color") {
        
    } else if (name == "texture") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        nString texturePath = value->data[0].as<nString>();

        vg::Texture tex = textureCache->addTexture(texturePath);

        label->setLabelTexture(tex.id);
    } else if (name == "hover_texture") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        nString texturePath = value->data[0].as<nString>();

        vg::Texture tex = textureCache->addTexture(texturePath);

        label->setLabelHoverTexture(tex.id);
    } else if (name == "text_color") {
        
    } else if (name == "text_hover_color") {

    }

    return vui::parseTextWidgetEntry(context, label, name, value, widgetParser);
}
