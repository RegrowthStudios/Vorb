#include "stdafx.h"
#include "graphics/Renderer.h"
#include "graphics/Scene.h"
#include "graphics/PostProcess.h"
#include "graphics/GBuffer.h"

vg::Renderer::Renderer() {
    // Empty
}

vg::Renderer::~Renderer() {
    if (!m_isInitialized) {
        dispose();
    }
}

bool vg::Renderer::init(vui::GameWindow* window) {

    vorb_assert(!m_isInitialized, "Renderer was initialized twice.");

    m_window = window;
    m_isInitialized = true;

    // Set up GBuffer for deferred rendering.
    m_gBuffer = std::make_unique<vg::GBuffer>(m_window->getWidth(), m_window->getHeight());

    // TODO(Ben): More than color.
    std::vector<GBufferAttachment> attachments;
    { // Color
        attachments.emplace_back();
        GBufferAttachment& color = attachments.back();
        color.format = GBUFFER_INTERNAL_FORMAT_COLOR;
        color.pixelFormat = vg::TextureFormat::RGBA;
        color.pixelType = vg::TexturePixelType::FLOAT;
        color.number = 0;
    }
    m_gBuffer->init(attachments, GBUFFER_INTERNAL_FORMAT_LIGHT);
    m_gBuffer->initDepth();
}

void vg::Renderer::load() {
    for (IScene* s : m_sceneLoadQueue) {
        s->load();
        m_scenes.push_back(s);
    }
    m_sceneLoadQueue.clear();

    for (IPostProcess* p : m_postProcessesLoadQueue) {
        p->load();
        m_postProcesses.push_back(p);
    }
    m_postProcessesLoadQueue.clear();
}

void vg::Renderer::registerScene(IScene* scene) {
    // Make sure the scene doesn't exist
    vorb_assert(std::find(m_scenes.begin(), m_scenes.end(), scene) == m_scenes.end(), "Scene already added to SceneRenderer.");

    scene->m_renderer = this;

    m_sceneLoadQueue.push_back(scene);
}

bool vg::Renderer::unregisterScene(IScene* scene) {
    auto& it = std::find(m_scenes.begin(), m_scenes.end(), scene);
    if (it != m_scenes.end()) {
        // Remove the scene
        m_scenes.erase(it);
        scene->m_renderer = nullptr;
        return true;
    } else {
        // Check the load list instead
        auto& it2 = std::find(m_sceneLoadQueue.begin(), m_sceneLoadQueue.end(), scene);
        if (it != m_scenes.end()) {
            m_sceneLoadQueue.erase(it2);
            scene->m_renderer = nullptr;
            return true;
        }
    }
    return false;
}

void vg::Renderer::registerPostProcess(IPostProcess* postProcess) {
    // Make sure the PostProcess doesn't exist
    vorb_assert(std::find(m_postProcesses.begin(), m_postProcesses.end(), postProcess) == m_postProcesses.end(), "PostProcess already added to SceneRenderer.");

    postProcess->m_renderer = this;

    m_postProcessesLoadQueue.push_back(postProcess);
}

bool vg::Renderer::unregisterPostProcess(IPostProcess* postProcess) {
    auto& it = std::find(m_postProcesses.begin(), m_postProcesses.end(), postProcess);
    if (it != m_postProcesses.end()) {
        // Remove the PostProcess
        m_postProcesses.erase(it);
        postProcess->m_renderer = nullptr;
        return true;
    } else {
        // Check the load list instead
        auto& it2 = std::find(m_postProcessesLoadQueue.begin(), m_postProcessesLoadQueue.end(), postProcess);
        if (it != m_postProcesses.end()) {
            m_postProcessesLoadQueue.erase(it2);
            postProcess->m_renderer = nullptr;
            return true;
        }
    }
    return false;
}

void vg::Renderer::setBackgroundColor(const color4& color) {
    setBackgroundColor(f32v4((f32)color.r * 255.0f,
                             (f32)color.g * 255.0f,
                             (f32)color.b * 255.0f,
                             (f32)color.a * 255.0f));
}

void vg::Renderer::setBackgroundColor(const f32v4& color) {
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    glClearColor(color.r, color.g, color.b, color.a);
#else
    throw "Only OpenGL background color is supported by the renderer"
#endif
}

void vg::Renderer::beginRenderFrame() {

    // Bind the GBuffer for drawing
    m_gBuffer->useGeometry();

#if defined(VORB_IMPL_GRAPHICS_OPENGL)

    // TODO(Ben): Allow optional clearing
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

#elif defined(VORB_IMPL_GRAPHICS_D3D)

    {

#if defined(VORB_DX_9)

        D3DVIEWPORT9 vp;
        vp.X = 0;
        vp.Y = 0;
        vp.Width = m_window.getWidth();
        vp.Height = m_window.getHeight();
        vp.MinZ = 0.0f;
        vp.MaxZ = 1.0f;
        VG_DX_DEVICE(m_window.getContext())->SetViewport(&vp);

#endif

    }

#if defined(VORB_DX_9)

    VG_DX_DEVICE(m_window.getContext())->BeginScene();

#endif
#endif
}

void vg::Renderer::renderScenes(const vui::GameTime& gameTime) {
    for (IScene* s : m_scenes) {
        s->render(gameTime);
    }
}

void vg::Renderer::renderPostProcesses() {

    // Disable FBO (Render to screen)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_window->getWidth(), m_window->getHeight());

    // Bind color
    m_gBuffer->bindGeometryTexture(0, 0);

    // Do all post processing
    for (IPostProcess* p : m_postProcesses) {
        p->render();
    }

    // If we have no post processes, we have to render the g buffer to the screen manually.
    if (m_postProcesses.empty()) {
        // Lazy load
        if (!m_postProcessPassthrough) {
            m_postProcessPassthrough = std::make_unique<vg::PostProcessPassthrough>();
            m_postProcessPassthrough->init(0);
            m_postProcessPassthrough->load();
        }
        // Bind color
        m_gBuffer->bindGeometryTexture(0, 0);
        // Render color to screen
        m_postProcessPassthrough->render();
    }

}

void vg::Renderer::dispose() {
    for (IScene* s : m_scenes) {
        s->dispose();
    }
    std::vector<IScene*>().swap(m_scenes);
    for (IScene* s : m_sceneLoadQueue) {
        s->dispose();
    }
    std::vector<IScene*>().swap(m_sceneLoadQueue);

    for (IPostProcess* p : m_postProcesses) {
        p->dispose();
    }
    std::vector<IPostProcess*>().swap(m_postProcesses);
    for (IPostProcess* p : m_postProcessesLoadQueue) {
        p->dispose();
    }
    std::vector<IPostProcess*>().swap(m_postProcessesLoadQueue);

    if (m_postProcessPassthrough) {
        m_postProcessPassthrough->dispose();
        m_postProcessPassthrough.reset();
        m_postProcessPassthrough = nullptr;
    }

    m_isInitialized = false;
}
