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
#include "Vorb/ui/widgets/yaml/Widget.h"
#include "Vorb/ui/widgets/yaml/Label.h"
#include "Vorb/ui/widgets/yaml/CheckBox.h"
#include "Vorb/ui/widgets/yaml/ComboBox.h"
#include "Vorb/ui/widgets/yaml/Label.h"
#include "Vorb/ui/widgets/yaml/Panel.h"
#include "Vorb/ui/widgets/yaml/Slider.h"
#include "Vorb/ui/widgets/yaml/WidgetList.h"

// TODO(Matthew): Could be a lot cleaner if done entirely through Keg, but as is Keg doesn't seem to support
//                callbacks to perform setting rather than direct memory offsets. In the future Keg could be
//                extended to do that and this rewritten.

// TODO(Matthew): On failed parse, ensure clean up covers all thus-far created widgets!

vui::IWidget* parseWidget(keg::ReadContext& context, const nString& type, keg::Node node, vui::UILoader::WidgetParser* customWidgetParser) {
    // Ensure node is a map.
    if (keg::getType(node) != keg::NodeType::MAP) return nullptr;

    // Define a functor that provides both default parsing and, on failure of default parsing, pass off 
    // to custom parser.
    auto widgetParser = makeFunctor([&](const nString& type, keg::Node node) {
        vui::IWidget* widget = parseWidget(context, type, node, customWidgetParser);

        if (!widget && customWidgetParser) widget = customWidgetParser->invoke(type, node);

        return widget;
    });

    // For each entry in the map, create the appropriate widget and send it off for parsing to the correct parser.
    bool success = true;
    Delegate<void, Sender, const nString&, keg::Node> processEntry;
    vui::IWidget* widget;
    if (type == "button") {
        widget = new vui::Button();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = success && vui::parseButtonEntry(context, static_cast<vui::Button*>(widget), key, value, widgetParser);
        });
    } else if (type == "checkbox") {
        widget = new vui::CheckBox();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = success && vui::parseCheckBoxEntry(context, static_cast<vui::CheckBox*>(widget), key, value, widgetParser);
        });
    } else if (type == "combobox") {
        widget = new vui::ComboBox();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = success && vui::parseComboBoxEntry(context, static_cast<vui::ComboBox*>(widget), key, value, widgetParser);
        });
    } else if (type == "label") {
        widget = new vui::Label();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = success && vui::parseLabelEntry(context, static_cast<vui::Label*>(widget), key, value, widgetParser);
        });
    } else if (type == "panel") {
        widget = new vui::Panel();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = success && vui::parsePanelEntry(context, static_cast<vui::Panel*>(widget), key, value, widgetParser);
        });
    } else if (type == "slider") {
        widget = new vui::Slider();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = success && vui::parseSliderEntry(context, static_cast<vui::Slider*>(widget), key, value, widgetParser);
        });
    } else if (type == "widget_list") {
        widget = new vui::WidgetList();
        processEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
            success = success && vui::parseWidgetListEntry(context, static_cast<vui::WidgetList*>(widget), key, value, widgetParser);
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

bool parseViewport(keg::ReadContext& context, vui::Viewport* viewport, keg::Node node, vui::UILoader::WidgetParser* customWidgetParser) {
    // Ensure node is a map.
    if (keg::getType(node) != keg::NodeType::MAP) return false;

    // Define a functor that provides both default parsing and, on failure of default parsing, pass off 
    // to custom parser.
    auto widgetParser = makeFunctor([&](const nString& type, keg::Node node) {
        vui::IWidget* widget = parseWidget(context, type, node, customWidgetParser);

        if (!widget && customWidgetParser) widget = customWidgetParser->invoke(type, node);

        return widget;
    });

    // For each entry in the map, send it off for parsing.
    bool success = true;
    auto processViewportEntry = makeFunctor([&](Sender, const nString& key, keg::Node value) {
        success = success && vui::parseWidgetEntry(context, viewport, key, value, &widgetParser);
    });
    context.reader.forAllInMap(node, &processViewportEntry);

    return success;
}

bool vui::UILoader::load(const vio::IOManager& iom, const cString filepath, OUT Viewport& viewport, WidgetParser* customWidgetParser /*= nullptr*/) {
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

    // Get first node entry and pass on for parsing as viewport.
    keg::Node node = context.reader.getFirst();
    bool res = parseViewport(context, &viewport, node, customWidgetParser);

    // Dispose reader and return result.
    context.reader.dispose();
    return res;
}

bool vui::UILoader::save(const cString filepath, const Viewport& viewport) {

}
