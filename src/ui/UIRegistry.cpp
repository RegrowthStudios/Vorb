#include "Vorb/stdafx.h"
#include "Vorb/ui/UIRegistry.h"

void vui::UIRegistryBase::init(const GameWindow* window, vg::TextureCache* textureCache, vg::FontCache* fontCache) {
    m_window = window;
    m_textureCache = textureCache;
    m_fontCache = fontCache;
}

void vui::UIRegistryBase::dispose() {
    m_window       = nullptr;
    m_textureCache = nullptr;
    m_fontCache    = nullptr;

    RegisteredUIs().swap(m_uis);
}

bool vui::UIRegistryBase::registerUI(const nString& name, UIBase* ui) {
    return registerUI(std::make_pair(name, ui));
}

bool vui::UIRegistryBase::registerUI(std::pair<nString, UIBase*>&& ui) {
    return m_uis.emplace(std::forward<std::pair<nString, UIBase*>>(ui)).second;
}

vui::UIBase* vui::UIRegistryBase::getUI(const nString& name) {
    try {
        return m_uis.at(name);
    } catch(std::out_of_range& e) {
        return nullptr;
    }
}
