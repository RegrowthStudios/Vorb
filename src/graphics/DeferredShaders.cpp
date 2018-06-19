#include "Vorb/stdafx.h"
#include "Vorb/graphics/DeferredShaders.h"

vg::DeferredShaders::DeferredShaders() :
    clear(false),
    composition(false) {
    // Empty
}

void vg::DeferredShaders::dispose() {
    clear.dispose();
    composition.dispose();

    for (auto& it : geometry) it.second.dispose();
    std::unordered_map<nString, GLProgram>().swap(geometry);

    for (auto& it : light) it.second.dispose();
    std::unordered_map<nString, GLProgram>().swap(light);
}
