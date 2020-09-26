#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/WidgetList.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/WidgetList.h"
#include "Vorb/ui/widgets/yaml/Parser.h"
#include "Vorb/ui/widgets/yaml/Widget.h"

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
        if (keg::getType(value) != keg::NodeType::MAP) return false;

        bool success = true;
        auto handleWidgetParse = makeFunctor([&](Sender, const nString& type, keg::Node node) {
            vui::IWidget* child = nullptr;
            if (keg::getType(node) != keg::NodeType::MAP &&
                    node->data.size() == 2 &&
                        node->data["index"] && node->data["item"]) {
                keg::YAMLNode tmp = { node->data["item"] };
                child = widgetParser->invoke(type, &tmp);

                widgetList->addItemAtIndex(node->data["index"].as<size_t>(), child);
            } else {
                child = widgetParser->invoke(type, node);

                widgetList->addItem(child);
            }

            success = success && (child != nullptr);
        });
        context.reader.forAllInMap(value, &handleWidgetParse);

        return success;
    } else {
        return vui::parseWidgetEntry(context, widgetList, name, value, widgetParser);
    }

    return true;
}
