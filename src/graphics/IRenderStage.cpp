#include "stdafx.h"
#include "graphics/IRenderStage.h"
#include "graphics/GLProgram.h"

vg::IRenderStage::IRenderStage(const nString& displayName /* = "" */, const Camera* camera /* = nullptr */) :
    m_name(displayName),
    m_camera(camera) {
    // Empty
}


vg::IRenderStage::~IRenderStage() {
    // Empty
}

void vorb::graphics::IRenderStage::reloadShader() {
    if (m_program.isCreated()) m_program.dispose();
}

void vorb::graphics::IRenderStage::dispose() {
    if (m_program.isCreated()) m_program.dispose();
    m_isVisible = false;
}
