#include "stdafx.h"
#include "graphics/RenderPipeline.h"
#include "graphics/IRenderStage.h"

vg::RenderPipeline::RenderPipeline() {
    // Empty
}

vg::RenderPipeline::~RenderPipeline() {
    // Empty
}

void vorb::graphics::RenderPipeline::setCamera(Camera* camera) {
    m_camera = camera;
    for (auto& stage : m_stages) {
        stage->setCamera(camera);
    }
}

void vorb::graphics::RenderPipeline::addStage(StagePtr stage) {
    m_stages.push_back(stage);
}

void vorb::graphics::RenderPipeline::render() {
    for (auto& stage : m_stages) {
        if (stage->isVisible()) stage->render();
    }
}

void vorb::graphics::RenderPipeline::destroy(bool shouldDisposeStages) {
    if (shouldDisposeStages) {
        disposeStages();
    }
    for (auto& stage : m_stages) {
        stage.reset();
    }
    std::vector <StagePtr>().swap(m_stages);
}

void vorb::graphics::RenderPipeline::disposeStages() {
    for (auto& stage : m_stages) {
        stage->dispose();
    }
}

void vorb::graphics::RenderPipeline::reloadShaders() {
    for (auto& stage : m_stages) {
        stage->reloadShader();
    }
}
