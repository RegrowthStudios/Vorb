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

    } else if (name == "raw_rel_pos") {

    } else if (name == "raw_min_size") {

    } else if (name == "raw_max_size") {

    } else if (name == "raw_dock_size") {

    } else if (name == "raw_padding") {

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
