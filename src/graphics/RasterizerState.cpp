#include "Vorb/stdafx.h"
#include "Vorb/graphics/RasterizerState.h"

vg::RasterizerState::RasterizerState(bool use, CullFaceMode cullFaceMode, FrontFaceDirection frontFaceDirection) :
useCulling(use),
cullMode(cullFaceMode),
faceOrientation(frontFaceDirection) {}

void vg::RasterizerState::set() const {
    if (useCulling) {
        glEnable(GL_CULL_FACE);
        glFrontFace(static_cast<GLenum>(faceOrientation));
        glCullFace(static_cast<GLenum>(cullMode));
    } else {
        glDisable(GL_CULL_FACE);
    }
}

const vg::RasterizerState vg::RasterizerState::CULL_NONE(false, vg::CullFaceMode::BACK, vg::FrontFaceDirection::CCW);
const vg::RasterizerState vg::RasterizerState::CULL_CLOCKWISE(true, vg::CullFaceMode::BACK, vg::FrontFaceDirection::CCW);
const vg::RasterizerState vg::RasterizerState::CULL_COUNTER_CLOCKWISE(true, vg::CullFaceMode::BACK, vg::FrontFaceDirection::CW);