#include "Vorb/stdafx.h"
#include "Vorb/ui/ScriptedUI.h"

#include "Vorb/ui/Widget.h"
#include "Vorb/ui/ViewScriptEnvironment.h"

template <typename ScriptEnvironmentImpl>
vui::ScriptedUI<ScriptEnvironmentImpl>::ScriptedUI() :
    m_window(nullptr),
    m_defaultFont(nullptr),
    m_spriteBatch(nullptr) {
    // Empty
}

template <typename ScriptEnvironmentImpl>
vui::ScriptedUI<ScriptEnvironmentImpl>::~ScriptedUI() {
    // Empty
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::init(const GameWindow* window, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_window      = window;
    m_defaultFont = defaultFont;
    m_spriteBatch = spriteBatch;
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::dispose() {
    for (auto& view : m_views) {
        view.second.viewport->dispose();
        view.second.scriptEnv->dispose();
        delete view.second.viewport;
        delete view.second.scriptEnv;
    }
    UIViews().swap(m_views);

    m_window      = nullptr;
    m_defaultFont = nullptr;
    m_spriteBatch = nullptr;
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::update(f32 dt /*= 1.0f*/) {
    for (auto& view : m_views) {
        view.second.scriptEnv->update(dt);
        view.second.viewport->update(dt);
    }
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::draw() {
    for (auto& view : m_views) {
        view.second.viewport->draw();
    }
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::onOptionsChanged() {
    // TODO(Matthew): Implement.
}

template <typename ScriptEnvironmentImpl>
vui::ScriptedView<ScriptEnvironmentImpl> vui::ScriptedUI<ScriptEnvironmentImpl>::makeView(const nString& name, ZIndex zIndex, const f32v4& dimensions /*= f32v4(0.0f)*/, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    ScriptedView<ScriptEnvironmentImpl> view;

    view.viewport = new Viewport(m_window);
    view.viewport->init(name, dimensions, defaultFont ? defaultFont : m_defaultFont, spriteBatch ? spriteBatch : m_spriteBatch);

    view.scriptEnv = new ScriptEnv();
    view.scriptEnv->init(viewport, m_window);

    prepareScriptEnv(view.scriptEnv);

    m_views.insert(std::make_pair(zIndex, view));

    return view;
}

template <typename ScriptEnvironmentImpl>
vui::ScriptedView<ScriptEnvironmentImpl> vui::ScriptedUI<ScriptEnvironmentImpl>::makeView(const nString& name, ZIndex zIndex, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    ScriptedView<ScriptEnvironmentImpl> view;

    view.viewport = new Viewport(m_window);
    view.viewport->init(name, position, size, defaultFont ? defaultFont : m_defaultFont, spriteBatch ? spriteBatch : m_spriteBatch);

    view.scriptEnv = new ScriptEnv();
    view.scriptEnv->init(viewport, m_window);

    prepareScriptEnv(view.scriptEnv);

    m_views.insert(std::make_pair(zIndex, view));

    return view;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::makeViewFromScript(const nString& name, ZIndex zIndex, const vio::File& filepath) {
    ScriptedView<ScriptEnvironmentImpl> view = makeView(name, zIndex);

    view.scriptEnv.load(filepath);

    return view.viewport;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::makeViewFromScript(const nString& name, ZIndex zIndex, const vio::File& filepath) {
    ScriptedView<ScriptEnvironmentImpl> view = makeView(name, zIndex);

    // TODO(Matthew): Implement building view from YAML.

    return view.viewport;
}

// TODO(Matthew): Do we want to consider sorting by name, either not guaranteeing order of render of each viewport, or storing the information twice?
template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::getView(const nString& name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            return view.second.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::ViewScriptEnvironment<ScriptEnvironmentImpl>* vui::ScriptedUI<ScriptEnvironmentImpl>::getEnv(const nString& name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            return view.second.scriptEnv;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::enableView(const nString& name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            view.second.viewport->enable();
            return view.second.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::disableView(const nString& name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            view.second.viewport->disable();
            return view.second.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
bool vui::ScriptedUI<ScriptEnvironmentImpl>::destroyView(const nString& name) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            // Dispose the viewport and contained widgets and the script environment of the viewport.
            view.second.viewport->dispose();
            view.second.scriptEnv->dispose();

            // Free our memory.
            delete view.second.viewport;
            delete view.second.scriptEnv;

            // Erase view from the list of views.
            m_views.erase(view);

            return true;
        }
    }
    return false;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::setViewZIndex(const nString& name, ZIndex zIndex) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            Viewport* viewport = view.second.viewport;

            ScriptedView<ScriptEnvironmentImpl> newView;
            view.viewport  = viewport;
            view.scriptEnv = view.second.scriptEnv;

            m_views.erase(view);

            m_views.insert(std::make_pair(zIndex, newView));

            return viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
vui::Viewport* vui::ScriptedUI<ScriptEnvironmentImpl>::runViewScript(const nString& name, const vio::File& filepath) {
    for (auto& view : m_views) {
        if (view.second.viewport->getName() == name) {
            view.second.scriptEnv->load(filepath);
            return view.second.viewport;
        }
    }
    return nullptr;
}

template <typename ScriptEnvironmentImpl>
void vui::ScriptedUI<ScriptEnvironmentImpl>::prepareScriptEnv(ScriptEnv* scriptEnv) {
    vscript::IEnvironment<ScriptEnvironmentImpl>* env = scriptEnv->getEnv();

    env->setNamespaces("UI");
    env->addCDelegate("makeViewFromScript", makeDelegate(this, &ScriptedUI::makeViewFromScript);
    env->addCDelegate("makeViewFromYAML",   makeDelegate(this, &ScriptedUI::makeViewFromYAML));
    env->addCDelegate("enableView",         makeDelegate(this, &ScriptedUI::enableView));
    env->addCDelegate("disableView",        makeDelegate(this, &ScriptedUI::disableView));
    env->addCDelegate("getView",            makeDelegate(this, &ScriptedUI::getView));
    env->addCDelegate("setViewZIndex",      makeDelegate(this, &ScriptedUI::setViewZIndex));
    env->setNamespaces();
}
