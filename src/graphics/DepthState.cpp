#include "stdafx.h"
#include "graphics/DepthState.h"

vg::DepthState::DepthState(bool read, DepthFunction depthFunction, bool write) :
shouldRead(read),
shouldWrite(write),
depthFunc(depthFunction) {}

void vg::DepthState::set() const {
    if (shouldRead || shouldWrite) {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(shouldWrite);
        glDepthFunc(static_cast<GLenum>(depthFunc));
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

const vg::DepthState vg::DepthState::FULL(true, vg::DepthFunction::LESS, true);
const vg::DepthState vg::DepthState::WRITE(false, vg::DepthFunction::ALWAYS, true);
const vg::DepthState vg::DepthState::READ(true, vg::DepthFunction::LESS, false);
const vg::DepthState vg::DepthState::NONE(false, vg::DepthFunction::ALWAYS, false);

