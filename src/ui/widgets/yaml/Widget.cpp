#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/Widget.h"

#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/widgets/yaml/IWidget.h"

bool vui::parsePositionType(const nString& name, OUT vui::PositionType& positionType) {
    if (name == "static_to_window") {
        positionType = vui::PositionType::STATIC_TO_WINDOW;
        return true;
    } else if (name == "static_to_viewport") {
        positionType = vui::PositionType::STATIC_TO_VIEWPORT;
        return true;
    } else if (name == "static_to_parent") {
        positionType = vui::PositionType::STATIC_TO_PARENT;
        return true;
    } else if (name == "relative_to_window") {
        positionType = vui::PositionType::RELATIVE_TO_WINDOW;
        return true;
    } else if (name == "relative_to_viewport") {
        positionType = vui::PositionType::RELATIVE_TO_VIEWPORT;
        return true;
    } else if (name == "relative_to_parent") {
        positionType = vui::PositionType::RELATIVE_TO_PARENT;
        return true;
    }
    return false;
}

bool vui::parseDimensionType(const nString& name, OUT vui::DimensionType& dimensionType) {
    if (name == "pixel") {
        dimensionType = vui::DimensionType::PIXEL;
        return true;
    } else if (name == "width_percentage") {
        dimensionType = vui::DimensionType::WIDTH_PERCENTAGE;
        return true;
    } else if (name == "height_percentage") {
        dimensionType = vui::DimensionType::HEIGHT_PERCENTAGE;
        return true;
    } else if (name == "min_percentage") {
        dimensionType = vui::DimensionType::MIN_PERCENTAGE;
        return true;
    } else if (name == "max_percentage") {
        dimensionType = vui::DimensionType::MAX_PERCENTAGE;
        return true;
    } else if (name == "parent_width_percentage") {
        dimensionType = vui::DimensionType::PARENT_WIDTH_PERCENTAGE;
        return true;
    } else if (name == "parent_height_percentage") {
        dimensionType = vui::DimensionType::PARENT_HEIGHT_PERCENTAGE;
        return true;
    } else if (name == "parent_min_percentage") {
        dimensionType = vui::DimensionType::PARENT_MIN_PERCENTAGE;
        return true;
    } else if (name == "parent_max_percentage") {
        dimensionType = vui::DimensionType::PARENT_MAX_PERCENTAGE;
        return true;
    } else if (name == "viewport_width_percentage") {
        dimensionType = vui::DimensionType::VIEWPORT_WIDTH_PERCENTAGE;
        return true;
    } else if (name == "viewport_height_percentage") {
        dimensionType = vui::DimensionType::VIEWPORT_HEIGHT_PERCENTAGE;
        return true;
    } else if (name == "viewport_min_percentage") {
        dimensionType = vui::DimensionType::VIEWPORT_MIN_PERCENTAGE;
        return true;
    } else if (name == "viewport_max_percentage") {
        dimensionType = vui::DimensionType::VIEWPORT_MAX_PERCENTAGE;
        return true;
    } else if (name == "window_width_percentage") {
        dimensionType = vui::DimensionType::WINDOW_WIDTH_PERCENTAGE;
        return true;
    } else if (name == "window_height_percentage") {
        dimensionType = vui::DimensionType::WINDOW_HEIGHT_PERCENTAGE;
        return true;
    } else if (name == "window_min_percentage") {
        dimensionType = vui::DimensionType::WINDOW_MIN_PERCENTAGE;
        return true;
    } else if (name == "window_max_percentage") {
        dimensionType = vui::DimensionType::WINDOW_MAX_PERCENTAGE;
        return true;
    }
    return false;
}

bool vui::parseLength(keg::Node node, OUT vui::Length& length) {
    if (keg::getType(node) != keg::NodeType::MAP
            || node->data.size() != 2) return false;

    if (!(node->data["x"] && node->data["dim_x"])) return false;

    vui::DimensionType dimType;
    if (!parseDimensionType(node->data["dim_x"].as<nString>(), dimType)) return false;

    length.x = node->data["x"].as<f32>();
    length.dimension.x = dimType;

    return true;
}

bool vui::parseLength2(keg::Node node, OUT vui::Length2& length) {
    if (keg::getType(node) != keg::NodeType::MAP
            || node->data.size() != 4) return false;

    if (!(node->data["x"] && node->data["dim_x"]
            && node->data["y"] && node->data["dim_y"])) return false;

    vui::DimensionType dimType1, dimType2;
    if (!(parseDimensionType(node->data["dim_x"].as<nString>(), dimType1)
            && parseDimensionType(node->data["dim_y"].as<nString>(), dimType2))) return false;

    length.x = node->data["x"].as<f32>();
    length.y = node->data["y"].as<f32>();
    length.dimension.x = dimType1;
    length.dimension.y = dimType2;

    return true;
}

bool vui::parseLength4(keg::Node node, OUT vui::Length4& length) {
    if (keg::getType(node) != keg::NodeType::MAP
            || node->data.size() != 8) return false;

    if (!(node->data["x"] && node->data["dim_x"]
            && node->data["y"] && node->data["dim_y"]
            && node->data["z"] && node->data["dim_z"]
            && node->data["w"] && node->data["dim_w"])) return false;

    vui::DimensionType dimType1, dimType2, dimType3, dimType4;
    if (!(parseDimensionType(node->data["dim_x"].as<nString>(), dimType1)
            && parseDimensionType(node->data["dim_y"].as<nString>(), dimType2)
            && parseDimensionType(node->data["dim_z"].as<nString>(), dimType3)
            && parseDimensionType(node->data["dim_w"].as<nString>(), dimType4))) return false;

    length.x = node->data["x"].as<f32>();
    length.y = node->data["y"].as<f32>();
    length.z = node->data["z"].as<f32>();
    length.w = node->data["w"].as<f32>();
    length.dimension.x = dimType1;
    length.dimension.y = dimType2;
    length.dimension.z = dimType3;
    length.dimension.w = dimType4;

    return true;
}

bool vui::parseWidgetEntry(keg::ReadContext& context, vui::Widget* widget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser) {
    if (name == "positioning") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        vui::PositionType positionType;
        if (!parsePositionType(value->data[0].as<nString>(), positionType)) return false;

        widget->setPositionType(positionType);
    } else if (name == "raw_dimensions") {
        if (keg::getType(value) != keg::NodeType::MAP
                || value->data.size() != 2) return false;

        if (!(value->data["position"] && value->data["size"])) return false;

        vui::Length2 position, size;

        keg::YAMLNode tmp  = { value->data["position"] };
        keg::Node     node = &tmp;
        if (!parseLength2(node, position)) return false;

        tmp = { value->data["size"] };
        if (!parseLength2(node, size)) return false;

        widget->setRawPosition(position);
        widget->setRawSize(size);
    } else if (name == "raw_rel_pos") {
        if (keg::getType(value) != keg::NodeType::MAP
                || value->data.size() != 4) return false;

        if (!(value->data["left"] && value->data["top"])
                && value->data["right"] && value->data["bottom"])) return false;

        vui::Length left, top, right, bottom;

        keg::YAMLNode tmp  = { value->data["left"] };
        keg::Node     node = &tmp;
        if (!parseLength(node, left)) return false;

        tmp = { value->data["top"] };
        if (!parseLength(node, top)) return false;

        tmp = { value->data["right"] };
        if (!parseLength(node, right)) return false;

        tmp = { value->data["bottom"] };
        if (!parseLength(node, bottom)) return false;

        widget->setRawLeft(left);
        widget->setRawTop(top);
        widget->setRawRight(right);
        widget->setRawBottom(bottom);
    } else if (name == "raw_min_size") {
        vui::Length2 minSize;

        if (!parseLength2(value, minSize)) return false;

        widget->setRawMinSize(minSize);
    } else if (name == "raw_max_size") {
        vui::Length2 maxSize;

        if (!parseLength2(value, maxSize)) return false;

        widget->setRawMaxSize(maxSize);
    } else if (name == "raw_dock_size") {
        vui::Length dockSize;

        if (!parseLength(value, dockSize)) return false;

        widget->setRawDockSize(dockSize);
    } else if (name == "raw_padding") {
        vui::Length4 padding;

        if (!parseLength4(value, padding)) return false;

        widget->setRawPadding(padding);
    }

    return vui::parseIWidgetEntry(context, widget, name, value, widgetParser);
}
