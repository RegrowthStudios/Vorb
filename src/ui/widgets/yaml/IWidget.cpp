#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/IWidget.h"

#include "Vorb/ui/widgets/IWidget.h"
#include "Vorb/ui/widgets/yaml/helper.hpp"

bool vui::parseWidgetEntry(keg::ReadContext& context, vui::IWidget* widget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser) {
    if (name == "name") {
        nString name;
        if (!parseString(value, name)) return false;

        widget->setName(name);
    } else if (name == "size") {
        f32v2 size;
        if (!parseVec2(value, size)) return false;

        widget->setSize(size);
    } else if (name == "position") {
        f32v2 position;
        if (!parseVec2(value, position)) return false;

        widget->setPosition(position);
    } else if (name == "padding") {
        f32v4 padding;
        if (!parseVec4(value, padding)) return false;

        widget->setPadding(padding);
    } else if (name == "clipping") {
        vui::ClippingState clipping[4];
        if (!parseClipping(value, &clipping[0])) return false;

        widget->setClipping(vui::Clipping{ clipping[0], clipping[1], clipping[2], clipping[3] });
    } else if (name == "z_index") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        widget->setZIndex(value->data[0].as<vui::ZIndex>());
    } else if (name == "dock") {
        vui::Dock dock;
        if (!parseDock(value, dock)) return false;

        widget->setDock(dock);
    } else if (name == "child_offset") {
        f32v2 childOffset;
        if (!parseVec2(value, childOffset)) return false;

        widget->setChildOffset(childOffset);
    } else if (name == "widgets") {
        if (keg::getType(value) != keg::NodeType::MAP) return false;

        bool success = true;
        auto handleWidgetParse = makeFunctor([&](Sender, const nString& type, keg::Node node) {
            vui::IWidget* child = widgetParser->invoke(type, node);

            success = success && (child != nullptr);

            widget->addWidget(child);
        });
        context.reader.forAllInMap(value, &handleWidgetParse);

        return success;
    }
    return true;
}
