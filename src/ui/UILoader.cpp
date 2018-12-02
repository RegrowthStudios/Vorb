#include "Vorb/stdafx.h"
#include "Vorb/ui/UILoader.h"

#include "Vorb/Event.hpp"
#include "Vorb/io/IOManager.h"
#include "Vorb/io/Keg.h"
#include "Vorb/io/Path.h"
#include "Vorb/io/YAMLImpl.h"
#include "Vorb/ui/widgets/IWidget.h"
#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/widgets/Viewport.h"

bool vui::UILoader::load(const vio::IOManager& iom, const cString filepath, OUT Viewport& viewport) {
    // Check if the filepath given can be resolved.
    vio::Path path;
    if (!iom.resolvePath(filepath, path)) return false;

    // Read the file, check something was indeed read in.
    nString data;
    iom.readFileToString(path, data);
    if (data.empty()) return false;

    // Set up a YAML read context.
    keg::ReadContext context;
    context.env = keg::getGlobalEnvironment();
    context.reader.init(data.c_str());

    // Get first node entry and ensure it is a map (associative array).
    keg::Node node = context.reader.getFirst();
    if (keg::getType(node) != keg::NodeType::MAP) {
        context.reader.dispose();
        return false;
    }

    // We expect just one entry at the top level.
    if (node->data.size() != 1) return false;

    // Load viewport.
    bool res = loadViewport(context, &viewport, node);

    // Dispose reader and return result.
    context.reader.dispose();
    return res;
}

bool vui::UILoader::save(const cString filepath, const Viewport& viewport) {

}

bool parseClippingState(const nString& name, OUT vui::ClippingState& clippingState) {
    if (name == "visible") {
        clippingState = vui::ClippingState::VISIBLE;
        return true;
    } else if (name == "hidden") {
        clippingState = vui::ClippingState::HIDDEN;
        return true;
    } else if (name == "inherit") {
        clippingState = vui::ClippingState::INHERIT;
        return true;
    }
    return false;
}

bool parseDockState(const nString& name, OUT vui::DockState& dockState) {
    if (name == "none") {
        dockState = vui::DockState::NONE;
        return true;
    } else if (name == "left") {
        dockState = vui::DockState::LEFT;
        return true;
    } else if (name == "right") {
        dockState = vui::DockState::RIGHT;
        return true;
    } else if (name == "bottom") {
        dockState = vui::DockState::BOTTOM;
        return true;
    } else if (name == "top") {
        dockState = vui::DockState::TOP;
        return true;
    } else if (name == "fill") {
        dockState = vui::DockState::FILL;
        return true;
    }
    return false;
}

bool parsePositionType(const nString& name, OUT vui::PositionType& positionType) {
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

bool parseDimensionType(const nString& name, OUT vui::DimensionType& dimensionType) {
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

bool parseLength(keg::Node node, OUT vui::Length& length) {
    if (keg::getType(node) != keg::NodeType::MAP
            || node->data.size() != 2) return false;

    if (!(node->data["x"] && node->data["dim_x"])) return false;

    vui::DimensionType dimType;
    if (!parseDimensionType(node->data["dim_x"].as<nString>(), dimType)) return false;

    length.x = node->data["x"].as<f32>();
    length.dimension.x = dimType;

    return true;
}

bool parseLength2(keg::Node node, OUT vui::Length2& length) {
    if (keg::getType(node) != keg::NodeType::MAP
            || node->data.size() != 4) return false;

    if (!(node->data["x"] && node->data["dim_x"])
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

bool parseLength4(keg::Node node, OUT vui::Length4& length) {
    if (keg::getType(node) != keg::NodeType::MAP
            || node->data.size() != 8) return false;

    if (!(node->data["x"] && node->data["dim_x"])
            && node->data["y"] && node->data["dim_y"])
            && node->data["z"] && node->data["dim_z"])
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

bool parseWidgetEntry(vui::Widget* widget, const nString& name, keg::Node value) {
    if (name == "size") {
        if (keg::getType(value) != keg::NodeType::SEQUENCE
                || value->data.size() != 2) return false;

        f32v2 size = f32v2(value->data[0].as<f32>(), value->data[1].as<f32>());
        widget->setSize(size);
    } else if (name == "position") {
        if (keg::getType(value) != keg::NodeType::SEQUENCE
                || value->data.size() != 2) return false;

        f32v2 position = f32v2(value->data[0].as<f32>(), value->data[1].as<f32>());
        widget->setPosition(position);
    } else if (name == "padding") {
        if (keg::getType(value) != keg::NodeType::SEQUENCE
                || value->data.size() != 4) return false;

        f32v4 padding = f32v4(value->data[0].as<f32>(), value->data[1].as<f32>(),
                                value->data[2].as<f32>(), value->data[3].as<f32>());
        widget->setPadding(padding);
    } else if (name == "clipping") {
        if (keg::getType(value) != keg::NodeType::MAP
                || value->data.size() != 4) return false;

        vui::ClippingState clipping[4];
        if (!(parseClippingState(value->data["left"].as<nString>(), clipping[0])
                && parseClippingState(value->data["top"].as<nString>(), clipping[1])
                && parseClippingState(value->data["right"].as<nString>(), clipping[2])
                && parseClippingState(value->data["bottom"].as<nString>(), clipping[3]))) {
            return false;
        }
        widget->setClipping(vui::Clipping{ clipping[0], clipping[1], clipping[2], clipping[3] });
    } else if (name == "z_index") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        widget->setZIndex(value->data[0].as<vui::ZIndex>());
    } else if (name == "dock") {
        if (keg::getType(value) != keg::NodeType::MAP
                || value->data.size() != 2) return false;
        
        vui::DockState state;
        if (!parseDockState(value->data["state"].as<nString>(), state)) return false;

        if (!value->data["size"]) return false;
        widget->setDock(vui::Dock{ state, value->data["size"].as<f32>() });
    } else if (name == "child_offset") {
        if (keg::getType(value) != keg::NodeType::SEQUENCE
                || value->data.size() != 2) return false;

        f32v2 childOffset = f32v2(value->data[0].as<f32>(), value->data[1].as<f32>());
        widget->setChildOffset(childOffset);
    } else if (name == "positioning") {
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
    } else if (name == "widgets") {

    }
}

// vui::IWidget* loadWidget(const nString& name, keg::Node node) {
//     // If the widget node isn't a map, then return.
//     if (keg::getType(node) != keg::NodeType::MAP) return nullptr;
// }

bool loadViewport(keg::ReadContext& context, vui::Viewport* viewport, keg::Node node) {
    // Get Viewport node iterator.
    auto it = node->data.begin();

    // Set the viewport's name.
    viewport->setName(it->first.as<nString>());

    // Construct the node of the viewport.
    keg::YAMLNode tmp  = { it->second };
    keg::Node     node = &tmp;

    // Ensure node is a map.
    if (keg::getType(node) != keg::NodeType::MAP) return false;

    bool success = true;
    // For each entry in the map, send it off for parsing.
    auto processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
        success = success && parseWidgetEntry(viewport, key, value);
    });
    context.reader.forAllInMap(node, &processEntry);

    return success;
}
