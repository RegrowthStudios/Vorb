#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/Panel.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/Panel.h"
#include "Vorb/ui/widgets/yaml/helper.hpp"
#include "Vorb/ui/widgets/yaml/Widget.h"

bool vui::parsePanelEntry(keg::ReadContext& context, vui::Panel* panel, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache) {
    if (name == "bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        panel->setColor(color);
    } else if (name == "bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        panel->setHoverColor(color);
    } else if (name == "bg_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        panel->setTexture(tex.id);
    } else if (name == "bg_hover_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        panel->setHoverTexture(tex.id);
    } else if (name == "autoscroll") {
        bool autoscroll;
        if (!parseValue(*value, autoscroll)) return false;

        panel->setAutoScroll(autoscroll);
    } else if (name == "slider_width") {
        f32 width;
        if (!parseValue(*value, width)) return false;

        panel->setSliderWidth(width);
    } else {
        return vui::parseWidgetEntry(context, panel, name, value, widgetParser);
    }

    return true;
}
