#include "Vorb/stdafx.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/widgets/Viewport.h"
#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/GameWindow.h"

vui::Viewport::Viewport(const GameWindow* window /*= nullptr*/) :
    Widget(),
    m_window(window) {
    m_clipping = Clipping{ ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN };
}

vui::Viewport::Viewport(const Viewport& widget) :
    Widget(widget),
    m_renderer(widget.m_renderer),
    m_window(widget.m_window) {
    // Empty.
}

vui::Viewport::Viewport(Viewport&& widget) :
    Widget(std::forward<Viewport>(widget)),
    m_renderer(widget.m_renderer),
    m_window(widget.m_window) {
    // Empty.
}

vui::Viewport& vui::Viewport::operator=(const Viewport& rhs) {
    Widget::operator=(rhs);

    m_renderer = rhs.m_renderer;
    m_window   = rhs.m_window;

    return *this;
}

vui::Viewport& vui::Viewport::operator=(Viewport&& rhs) {
    Widget::operator=(std::forward<Viewport>(rhs));

    m_renderer = rhs.m_renderer;
    m_window   = rhs.m_window;

    return *this;
}

void vui::Viewport::init(const nString& name, const f32v4& dimensions, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    Widget::init(name, dimensions);

    m_viewport = this;
    updateDescendantViewports();

    m_positionType = PositionType::STATIC_TO_WINDOW;

    // Set defaults for all inheritable properties so that at worst getInheritedDefault hits here.
    m_inheritableGetterSetterMap["positionType"] = {
        makeFunctor([](IWidget*) {
            return any(DEFAULT_POSITION_TYPE);
        }),
        makeFunctor([](IWidget*, any) {
            // Empty.
        })
    };
    m_isModifiedMap["positionType"] = false;
    m_inheritableGetterSetterMap["font"] = {
        makeFunctor([defaultFont](IWidget*) {
            return any(const_cast<const vg::SpriteFont*>(defaultFont));
        }),
        makeFunctor([](IWidget*, any) {
            // Empty.
        })
    };
    m_isModifiedMap["font"] = false;
    m_inheritableGetterSetterMap["textColor"] = {
        makeFunctor([](IWidget*) {
            return any(color::Black);
        }),
        makeFunctor([](IWidget*, any) {
            // Empty.
        })
    };
    m_isModifiedMap["textColor"] = false;
    m_inheritableGetterSetterMap["textAlign"] = {
        makeFunctor([](IWidget*) {
            return any(DEFAULT_TEXT_ALIGN);
        }),
        makeFunctor([](IWidget*, any) {
            // Empty.
        })
    };
    m_isModifiedMap["textAlign"] = false;
    m_inheritableGetterSetterMap["textScale"] = {
        makeFunctor([](IWidget*) {
            return any(DEFAULT_TEXT_SCALE);
        }),
        makeFunctor([](IWidget*, any) {
            // Empty.
        })
    };
    m_isModifiedMap["textScale"] = false;

    m_renderer.init(defaultFont, spriteBatch);
}

void vui::Viewport::init(const nString& name, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    Widget::init(name, position, size);

    m_viewport = this;
    updateDescendantViewports();

    m_positionType = PositionType::STATIC_TO_WINDOW;

    m_renderer.init(defaultFont, spriteBatch);
}

void vui::Viewport::dispose(bool thisOnly /*= false*/) {
    IWidget::dispose(thisOnly);

    m_renderer.dispose();

    m_window = nullptr;
}

void vui::Viewport::enable() {
    if (!m_flags.isEnabled) {
        vui::InputDispatcher::window.onResize += makeDelegate(this, &Viewport::onResize);
    }

    Widget::enable();
}

void vui::Viewport::disable(bool thisOnly /*= false*/) {
    if (m_flags.isEnabled) {
        vui::InputDispatcher::window.onResize -= makeDelegate(this, &Viewport::onResize);
    }

    Widget::disable(thisOnly);
}

void vui::Viewport::update(f32 dt /*= 0.0f*/) {
    if (!m_flags.isEnabled) return;
    IWidget::update(dt);
    IWidget::updateDescendants(dt);
}

void vui::Viewport::draw() {
    if (!m_flags.isEnabled) return;
    m_renderer.prepare();

    addDescendantDrawables(m_renderer);

    m_renderer.render(f32v2(m_window->getViewportDims()));
}

void vui::Viewport::setGameWindow(const GameWindow* window) {
    m_window = window;
    
    m_flags.needsDimensionUpdate = true;
}

void vui::Viewport::onResize(Sender, const WindowResizeEvent&) {
    m_flags.needsDimensionUpdate = true;
    markDescendantsToUpdateDimensions();
}
