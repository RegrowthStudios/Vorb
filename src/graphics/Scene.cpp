#include "stdafx.h"
#include "graphics/Scene.h"
#include "graphics/Camera.h"
#include "graphics/Renderer.h"

vg::IScene::IScene() {
    // Empty
}

vg::IScene::~IScene() {
    // Empty
}

void vg::IScene::initCamera() {
    m_camera = std::make_unique<Camera>();
}

void vg::IScene::unregister() {
    vorb_assert(m_renderer, "Scene unregistered without having renderer.");

    m_renderer->unregisterScene(this);
}

void vg::IScene::dispose() {
    m_camera.reset();

    // Unregister if we need to.
    if (m_renderer) {
        m_renderer->unregisterScene(this);
    }
}