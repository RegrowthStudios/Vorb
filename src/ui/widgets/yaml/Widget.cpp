#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/Widget.h"

#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/widgets/yaml/helper.hpp"
#include "Vorb/ui/widgets/yaml/IWidget.h"

bool vui::parseWidgetEntry(keg::ReadContext& context, vui::Widget* widget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser) {
    if (name == "positioning") {
        vui::PositionType positionType;
        if (!parsePositionType(*value, positionType)) return false;

        widget->setPositionType(positionType);
    } else if (name == "raw_dimensions") {
        if (keg::getType(value) != keg::NodeType::MAP
                || value->data.size() != 2) return false;

        if (!(value->data["position"] && value->data["size"])) return false;

        vui::Length2 position, size;

        if (!parseLength2({ value->data["position"] }, position)) return false;

        if (!parseLength2({ value->data["size"] }, size)) return false;

        widget->setRawPosition(position);
        widget->setRawSize(size);
    } else if (name == "raw_rel_pos") {
        if (keg::getType(value) != keg::NodeType::MAP
                || value->data.size() != 4) return false;

        if (!(value->data["left"] && value->data["top"]
                && value->data["right"] && value->data["bottom"])) return false;

        vui::Length left, top, right, bottom;

        if (!parseLength({ value->data["left"]   }, left))   return false;
        if (!parseLength({ value->data["top"]    }, top))    return false;
        if (!parseLength({ value->data["right"]  }, right))  return false;
        if (!parseLength({ value->data["bottom"] }, bottom)) return false;

        widget->setRawLeft(left);
        widget->setRawTop(top);
        widget->setRawRight(right);
        widget->setRawBottom(bottom);
    } else if (name == "raw_min_size") {
        vui::Length2 minSize;

        if (!parseLength2(*value, minSize)) return false;

        widget->setRawMinSize(minSize);
    } else if (name == "raw_max_size") {
        vui::Length2 maxSize;

        if (!parseLength2(*value, maxSize)) return false;

        widget->setRawMaxSize(maxSize);
    } else if (name == "raw_dock_size") {
        vui::Length dockSize;

        if (!parseLength(*value, dockSize)) return false;

        widget->setRawDockSize(dockSize);
    } else if (name == "raw_padding") {
        vui::Length4 padding;

        if (!parseLength4(*value, padding)) return false;

        widget->setRawPadding(padding);
    }

    return vui::parseIWidgetEntry(context, widget, name, value, widgetParser);
}
