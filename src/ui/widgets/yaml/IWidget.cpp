#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/IWidget.h"

#include "Vorb/ui/widgets/IWidget.h"

bool vui::parseClippingState(const nString& name, OUT vui::ClippingState& clippingState) {
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

bool vui::parseDockState(const nString& name, OUT vui::DockState& dockState) {
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

bool vui::parseWidgetEntry(keg::ReadContext& context, vui::IWidget* widget, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser) {
    if (name == "name") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        widget->setName(value->data[0].as<nString>());
    } else if (name == "size") {
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
