#include "Vorb/stdafx.h"
#include "Vorb/ui/UILoader.h"

#include "Vorb/Event.hpp"
#include "Vorb/io/IOManager.h"
#include "Vorb/io/Keg.h"
#include "Vorb/io/Path.h"
#include "Vorb/io/YAMLImpl.h"
#include "Vorb/ui/widgets/Button.h"
#include "Vorb/ui/widgets/CheckBox.h"
#include "Vorb/ui/widgets/ComboBox.h"
#include "Vorb/ui/widgets/IWidget.h"
#include "Vorb/ui/widgets/Label.h"
#include "Vorb/ui/widgets/Panel.h"
#include "Vorb/ui/widgets/Slider.h"
#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/widgets/WidgetList.h"
#include "Vorb/ui/widgets/Viewport.h"
#include "Vorb/ui/widgets/yaml/Button.h"
#include "Vorb/ui/widgets/yaml/CheckBox.h"
#include "Vorb/ui/widgets/yaml/ComboBox.h"
#include "Vorb/ui/widgets/yaml/Label.h"
#include "Vorb/ui/widgets/yaml/Panel.h"
#include "Vorb/ui/widgets/yaml/Slider.h"
#include "Vorb/ui/widgets/yaml/Widget.h"
#include "Vorb/ui/widgets/yaml/WidgetList.h"

// TODO(Matthew): Could be a lot cleaner if done entirely through Keg, but as is Keg doesn't seem to support
//                callbacks to perform setting rather than direct memory offsets. In the future Keg could be
//                extended to do that and this rewritten.

// TODO(Matthew): Can we support YAML specifying Lua scripts to build specific widget (sub-trees) and/or specify
//                Lua functions to subscribe to events?

// TODO(Matthew): Can we put all the widgets in the same container to reduce fragmentation of their data?
//                Would involve creating some kind of generic container that is element-wise sized on max-sized widget kind.

vui::IWidget* parseWidget(keg::ReadContext& context, const nString& type, keg::Node node, vui::UILoader::WidgetParser* parser, vg::TextureCache* textureCache) {
    // Ensure node is a map.
    if (keg::getType(node) != keg::NodeType::MAP) return nullptr;

    // For each entry in the map, create the appropriate widget and send it off for parsing to the correct parser.
    bool success = true;
    Delegate<void, Sender, const nString&, keg::Node> processEntry;
    vui::IWidget* widget;
    if (type == "button") {
        widget = new vui::Button();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = vui::parseButtonEntry(context, static_cast<vui::Button*>(widget), key, value, parser, textureCache) && success;
        });
    } else if (type == "checkbox") {
        widget = new vui::CheckBox();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = vui::parseCheckBoxEntry(context, static_cast<vui::CheckBox*>(widget), key, value, parser, textureCache) && success;
        });
    } else if (type == "combobox") {
        widget = new vui::ComboBox();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = vui::parseComboBoxEntry(context, static_cast<vui::ComboBox*>(widget), key, value, parser, textureCache) && success;
        });
    } else if (type == "label") {
        widget = new vui::Label();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = vui::parseLabelEntry(context, static_cast<vui::Label*>(widget), key, value, parser, textureCache) && success;
        });
    } else if (type == "panel") {
        widget = new vui::Panel();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = vui::parsePanelEntry(context, static_cast<vui::Panel*>(widget), key, value, parser, textureCache) && success;
        });
    } else if (type == "slider") {
        widget = new vui::Slider();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = vui::parseSliderEntry(context, static_cast<vui::Slider*>(widget), key, value, parser, textureCache) && success;
        });
    } else if (type == "widget_list") {
        widget = new vui::WidgetList();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = vui::parseWidgetListEntry(context, static_cast<vui::WidgetList*>(widget), key, value, parser, textureCache) && success;
        });
    } else {
        return nullptr;
    }
    widget->init("");
    context.reader.forAllInMap(node, &processEntry);

    if (success) {
        return widget;
    } else {
        delete widget;
        return nullptr;
    }
}

bool parseViewport(keg::ReadContext& context, vui::Viewport* viewport, keg::Node node, vui::UILoader::WidgetParser* parser) {
    // Ensure node is a map.
    if (keg::getType(node) != keg::NodeType::MAP) return false;

    // For each entry in the map, send it off for parsing.
    bool success = true;
    auto processViewportEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
        success = vui::parseWidgetEntry(context, viewport, key, value, parser) && success;
    });
    context.reader.forAllInMap(node, &processViewportEntry);

    return success;
}


vui::IWidget* vui::UILoader::loadWidgetFromYAML(vio::IOManagerBase* ioManager, const cString filepath, vg::TextureCache* textureCache, WidgetParser* customWidgetParser /*= nullptr*/) {
    // Check if the filepath given can be resolved.
    vio::Path path;
    if (!ioManager->resolvePath(filepath, path)) return nullptr;

    // Read the file, check something was indeed read in.
    nString data;
    ioManager->readFileToString(path, data);
    if (data.empty()) return nullptr;

    // Set up a YAML read context.
    keg::ReadContext context;
    context.env = keg::getGlobalEnvironment();
    context.reader.init(data.c_str());

    // TODO(Matthew): Do we want to preferentially pass off to custom parser?
    // Define a functor that provides both default parsing and, on failure of default parsing, pass off 
    // to custom parser.
    auto widgetParser = makeFunctor([&](const nString& type, keg::Node node) {
        vui::IWidget* widget = parseWidget(context, type, node, customWidgetParser, textureCache);

        if (!widget && customWidgetParser) widget = customWidgetParser->invoke(type, node);

        return widget;
    });

    // Get first node entry and pass on for parsing as viewport.
    keg::Node node = context.reader.getFirst();

    // TODO(Matthew): Pretty sure this is wrong, the intent is to ensure there is one string at
    //                top-level of doc, corresponding to type of top-level widget of tree to be
    //                generated..
    if (keg::getType(node) != keg::NodeType::MAP
                || node->data.size() != 1) return nullptr;

    IWidget* res = nullptr;

    auto handleWidgetParse = makeFunctor([&](Sender, const nString& type, keg::Node node) {
        res = parseWidget(context, type, node, &widgetParser, textureCache);
    });
    context.reader.forAllInMap(node, &handleWidgetParse);

    // Dispose reader and return result.
    context.reader.dispose();
    return res;
}

bool vui::UILoader::loadViewFromYAML(Viewport* viewport, vio::IOManagerBase* ioManager, const cString filepath, vg::TextureCache* textureCache, WidgetParser* customWidgetParser /*= nullptr*/) {
    // Check if the filepath given can be resolved.
    vio::Path path;
    if (!ioManager->resolvePath(filepath, path)) return false;

    // Read the file, check something was indeed read in.
    nString data;
    ioManager->readFileToString(path, data);
    if (data.empty()) return false;

    // Set up a YAML read context.
    keg::ReadContext context;
    context.env = keg::getGlobalEnvironment();
    context.reader.init(data.c_str());

    // TODO(Matthew): Do we want to preferentially pass off to custom parser?
    // Define a functor that provides both default parsing and, on failure of default parsing, pass off 
    // to custom parser.
    auto widgetParser = makeFunctor([&](const nString& type, keg::Node node) {
        vui::IWidget* widget = parseWidget(context, type, node, customWidgetParser, textureCache);

        if (!widget && customWidgetParser) widget = customWidgetParser->invoke(type, node);

        return widget;
    });

    // Get first node entry and pass on for parsing as viewport.
    keg::Node node = context.reader.getFirst();
    bool res = parseViewport(context, viewport, node, &widgetParser);

    // Dispose reader and return result.
    context.reader.dispose();
    return res;
}
