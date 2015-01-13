#include "stdafx.h"
#include "graphics/DeferredShaders.h"

vg::DeferredShaders::DeferredShaders() :
    clear(false),
    composition(false) {
    // Empty
}

void vg::DeferredShaders::dispose() {
    clear.dispose();
    composition.dispose();
    std::unordered_map<nString, GLProgram> tmp1;
    std::unordered_map<nString, GLProgram> tmp2;

    for (auto& it : geometry) it.second.dispose();
    geometry.swap(tmp1);

    for (auto& it : light) it.second.dispose();
    light.swap(tmp2);
}
