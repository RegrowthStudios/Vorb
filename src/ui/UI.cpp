#include "stdafx.h"
#include "Vorb/ui/UI.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/graphics/SpriteBatch.h"
#include "Vorb/graphics/SpriteFont.h"
#include "Vorb/io/IOManager.h"
#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/IGameScreen.h"
#include "Vorb/ui/UILoader.h"
#include "Vorb/ui/widgets/Viewport.h"

vui::UI::UI() :
    m_window(nullptr),
    m_defaultFont(nullptr),
    m_spriteBatch(nullptr) {
    // Empty
}

vui::UI::~UI() {
    // Empty
}

void vui::UI::init(vui::IGameScreen* ownerScreen, const GameWindow* window, vio::IOManager* iom, vg::TextureCache* textureCache, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_ownerScreen  = ownerScreen;
    m_window       = window;
    m_iom          = iom;
    m_textureCache = textureCache;
    m_defaultFont  = defaultFont;
    m_spriteBatch  = spriteBatch;
}

void vui::UI::dispose() {
    for (auto& view : m_views) {
        view.second->dispose();
        delete view.second;
    }
    UIViews().swap(m_views);

    m_window      = nullptr;
    m_defaultFont = nullptr;
    m_spriteBatch = nullptr;
}

void vui::UI::update(f32 dt /*= 1.0f*/) {
    for (auto& view : m_views) {
        view.second->update(dt);
    }
}

void vui::UI::draw() {
    for (auto& view : m_views) {
        view.second->draw();
    }
}

void vui::UI::onOptionsChanged() {
    // TODO(Matthew): Implement.
}

vui::Viewport* vui::UI::makeView(const nString& name, ZIndex zIndex, const f32v4& dimensions /*= f32v4(0.0f)*/, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    Viewport* viewport = new Viewport(m_window);
    viewport->init(name, dimensions, defaultFont ? defaultFont : m_defaultFont, spriteBatch ? spriteBatch : m_spriteBatch);

    m_views.insert(std::make_pair(zIndex, viewport));

    return viewport;
}

vui::Viewport* vui::UI::makeView(const nString& name, ZIndex zIndex, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    Viewport* viewport = new Viewport(m_window);
    viewport->init(name, position, size, defaultFont ? defaultFont : m_defaultFont, spriteBatch ? spriteBatch : m_spriteBatch);

    m_views.insert(std::make_pair(zIndex, viewport));

    return viewport;
}

// TODO(Matthew): Limit time to make? Need to not get stuck on this, and in principle through
//                  file references we could easily get an infinite loop.
vui::Viewport* vui::UI::makeViewFromYAML(nString name, ZIndex zIndex, nString filepath) {
    Viewport* viewport = makeView(name, zIndex);

    if (!UILoader::loadFromYAML(m_iom, filepath.c_str(), m_textureCache, viewport)) {
        destroyView(viewport);

        viewport = nullptr;
    }

    return viewport;
}

// TODO(Matthew): Do we want to sort by name, either not guaranteeing order of render of each viewport, or storing the information twice?
vui::Viewport* vui::UI::getView(nString name) {
    for (auto& view : m_views) {
        if (view.second->getName() == name) {
            return view.second;
        }
    }
    return nullptr;
}

vui::Viewport* vui::UI::enableView(Viewport* viewport) {
    viewport->enable();

    return viewport;
}

vui::Viewport* vui::UI::enableViewWithName(nString name) {
    for (auto& view : m_views) {
        if (view.second->getName() == name) {
            view.second->enable();
            return view.second;
        }
    }
    return nullptr;
}

vui::Viewport* vui::UI::disableView(Viewport* viewport) {
    viewport->disable();

    return viewport;
}

vui::Viewport* vui::UI::disableViewWithName(nString name) {
    for (auto& view : m_views) {
        if (view.second->getName() == name) {
            view.second->disable();
            return view.second;
        }
    }
    return nullptr;
}


bool vui::UI::destroyView(Viewport* viewport) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second == viewport) {
            // Dispose the viewport and contained widgets and the script environment of the viewport.
            view.second->dispose();

            // Free our memory.
            delete view.second;

            // Erase view from the list of views.
            m_views.erase(it);

            return true;
        }
    }
    return false;
}

bool vui::UI::destroyViewWithName(nString name) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second->getName() == name) {
            // Dispose the viewport and contained widgets and the script environment of the viewport.
            view.second->dispose();

            // Free our memory.
            delete view.second;

            // Erase view from the list of views.
            m_views.erase(it);

            return true;
        }
    }
    return false;
}

vui::Viewport* vui::UI::setViewZIndex(nString name, ZIndex zIndex) {
    for (auto it = m_views.begin(); it != m_views.end(); ++it) {
        auto& view = *it;
        if (view.second->getName() == name) {
            Viewport* viewport  = view.second;

            m_views.erase(it);

            m_views.insert(std::make_pair(zIndex, viewport));

            return viewport;
        }
    }
    return nullptr;
}
