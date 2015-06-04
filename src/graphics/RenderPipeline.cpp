#include "stdafx.h"
#include "graphics/RenderPipeline.h"
#include "graphics/IRenderStage.h"

void vg::RenderPipeline::init(vui::GameWindow* window) {
    m_window = window;
    for (auto& stage : m_stages) {
        stage->init();
    }
}

void vg::RenderPipeline::dispose() {
    for (auto& stage : m_stages) {
        stage->dispose();
    }
    std::vector <IRenderStage*>().swap(m_stages);
}

void vg::RenderPipeline::render() {
    for (auto& stage : m_stages) {
        if (stage->isActive()) stage->render(m_camera);
    }
}

void vg::RenderPipeline::registerStage(IRenderStage* stage) {
    m_stages.push_back(stage);
}

void vg::RenderPipeline::setCamera(Camera* camera) {
    m_camera = camera;
}