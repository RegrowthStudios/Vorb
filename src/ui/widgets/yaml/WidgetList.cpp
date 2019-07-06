#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/WidgetList.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/WidgetList.h"
#include "Vorb/ui/widgets/yaml/helper.hpp"
#include "Vorb/ui/widgets/yaml/TextWidget.h"

bool vui::parseWidgetListEntry(keg::ReadContext& context, vui::WidgetList* widgetList, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache) {
    if (name == "bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        widgetList->setBackColor(color);
    } else if (name == "bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        widgetList->setBackHoverColor(color);
    } else if (name == "bg_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        widgetList->setTexture(tex.id);
    } else if (name == "spacing") {
        f32 spacing;
        if (!parseValue(*value, spacing)) return false;

        widgetList->setSpacing(spacing);
    } else if (name == "max_height") {
        f32 maxHeight;
        if (!parseValue(*value, maxHeight)) return false;

        widgetList->setSpacing(maxHeight);
    } else if (name == "children") {
        // TODO(Matthew): Implement adding/creating etc. children for widget list.
    } else {
        return vui::parseTextWidgetEntry(context, widgetList, name, value, widgetParser);
    }

    return true;
}
