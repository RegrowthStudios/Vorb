#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/CheckBox.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/CheckBox.h"
#include "Vorb/ui/widgets/yaml/Parser.h"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

bool vui::parseCheckBoxEntry(keg::ReadContext& context, vui::CheckBox* checkBox, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache) {
    if (name == "bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        checkBox->setBoxColor(color);
    } else if (name == "bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        checkBox->setBoxHoverColor(color);
    } else if (name == "bg_checked_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        checkBox->setBoxCheckedColor(color);
    } else if (name == "bg_checked_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        checkBox->setBoxCheckedHoverColor(color);
    } else if (name == "bg_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        checkBox->setUncheckedTexture(tex.id);
    } else if (name == "bg_hover_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        checkBox->setUncheckedHoverTexture(tex.id);
    } else if (name == "bg_checked_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        checkBox->setCheckedTexture(tex.id);
    } else if (name == "bg_checked_hover_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        checkBox->setCheckedHoverTexture(tex.id);
    } else if (name == "text_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        checkBox->setTextHoverColor(color);
    } else if (name == "checked") {
        bool checked;
        if (!parseValue(*value, checked)) return false;

        checkBox->setChecked(checked);
    } else {
        return vui::parseTextWidgetEntry(context, checkBox, name, value, widgetParser);
    }

    return true;
}
