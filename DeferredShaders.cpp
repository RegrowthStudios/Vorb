#include "stdafx.h"
#include "DeferredShaders.h"

vg::DeferredShaders::DeferredShaders() :
    clear(false),
    composition(false) {
    // Empty
}

void vg::DeferredShaders::dispose() {
    clear.dispose();
    composition.dispose();

    for (auto& it : geometry) it.second.dispose();
    geometry.swap(std::unordered_map<nString, GLProgram>());

    for (auto& it : light) it.second.dispose();
    light.swap(std::unordered_map<nString, GLProgram>());
}
