#include "Vorb/stdafx.h"
#include "Vorb/ui/UI.h"

#include "Vorb/ui/UILoader.h"

vui::UIBase::UIBase() :
    m_ownerScreen(nullptr),
    m_gameWindow(nullptr),
    m_ioManager(nullptr),
    m_textureCache(nullptr),
    m_fontCache(nullptr),
    m_spriteBatch(nullptr) {
    // Empty.
}

void vui::UIBase::init(
           IGameScreen* ownerScreen,
      const GameWindow* window,
    vio::IOManagerBase* ioManager,
      vg::TextureCache* textureCache,
         vg::FontCache* fontCache,
       vg::SpriteBatch* spriteBatch
) {
    m_ownerScreen  = ownerScreen;
    m_gameWindow   = window;
    m_ioManager    = ioManager;
    m_textureCache = textureCache,
    m_fontCache   = fontCache;
    m_spriteBatch  = spriteBatch;
}

void vui::UIBase::dispose() {
    m_ownerScreen  = nullptr;
    m_gameWindow   = nullptr;
    m_ioManager    = nullptr;
    m_textureCache = nullptr;
    m_fontCache   = nullptr;
    m_spriteBatch  = nullptr;

    for (auto& view : m_views) {
        view.second->dispose();
        delete view.second;
    }
    UIViews().swap(m_views);
}

void vui::UIBase::update(f32 dt = 0.0f) {
    PreUpdate(dt);

    for (auto& view : m_views) {
        view.second->update(dt);
    }

    PostUpdate(dt);
}

void vui::UIBase::draw() {
    PreDraw();

    for (auto& view : m_views) {
        view.second->draw();
    }

    PostDraw();
}

vui::Viewport* vui::UIBase::makeView(const nString& name, ZIndex zIndex, const f32v4& dimensions = f32v4(0.0f)) {
    Viewport* viewport = new Viewport(m_gameWindow);
    viewport->init(name, dimensions, m_fontCache, m_spriteBatch);

    m_views.insert(std::make_pair(zIndex, viewport));

    return viewport;
}

vui::Viewport* vui::UIBase::makeView(const nString& name, ZIndex zIndex, const Length2& position, const Length2& size) {
    Viewport* viewport = new Viewport(m_gameWindow);
    viewport->init(name, position, size, m_fontCache, m_spriteBatch);

    m_views.insert(std::make_pair(zIndex, viewport));

    return viewport;
}

vui::Viewport* vui::UIBase::makeViewFromYAML(const nString& name, ZIndex zIndex, const nString& filepath) {
    Viewport* viewport = makeView(name, zIndex);

    if (!UILoader::loadFromYAML(m_ioManager, filepath.c_str(), m_textureCache, viewport)) {
        destroyView(viewport);

        viewport = nullptr;
    }

    return viewport;
}

// TODO(Matthew): Do we want to also store views by name to make this cheaper?
vui::Viewport* vui::UIBase::getView(const nString& name) {
    for (auto& view : m_views) {
        if (view.second->getName() == name) {
            return view.second;
        }
    }
    return nullptr;
}

bool vui::UIBase::destroyView(Viewport* viewport) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second == viewport) {
            view.second->dispose();

            delete view.second;

            m_views.erase(it);

            return true;
        }
    }
    return false;
}

bool vui::UIBase::destroyViewWithName(const nString& name) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second->getName() == name) {
            view.second->dispose();

            delete view.second;

            m_views.erase(it);

            return true;
        }
    }
    return false;
}

bool vui::UIBase::setViewZIndex(Viewport* viewport, ZIndex zIndex) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second == viewport) {
            m_views.erase(it);

            m_views.insert(std::make_pair(zIndex, viewport));

            return true;
        }
    }
    return false;
}

bool vui::UIBase::setViewWithNameZIndex(const nString& name, ZIndex zIndex) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second->getName() == name) {
            Viewport* viewport  = view.second;

            m_views.erase(it);

            m_views.insert(std::make_pair(zIndex, viewport));

            return true;
        }
    }
    return false;
}