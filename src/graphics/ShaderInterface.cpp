#include "Vorb/stdafx.h"
#include "Vorb/graphics/ShaderInterface.h"
#include "Vorb/graphics/GLProgram.h"

i32 vg::ShaderInterface::build(const AttributeSemBinding& semBinds) {
    int numLinked = 0;
    // Set up VAO
    if (!m_vao) glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    // Loop through bindings
    for (auto& bind : m_bindings) {
        auto it = semBinds.find(bind.semantic);
        if (it != semBinds.end()) {
            bind.location = it->second;
            numLinked++;
            glEnableVertexAttribArray(bind.location);
            glVertexAttribPointer(bind.location, bind.size,
                                  static_cast<VGEnum>(bind.type),
                                  bind.normalized, bind.stride,
                                  (GLvoid*)bind.offset);
        } else {
            bind.location = -1;
        }
    }
    glBindVertexArray(0);

    return numLinked;
}

void vg::ShaderInterface::dispose() {
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    AttributeBindings().swap(m_bindings);
}

i32 vg::ShaderInterface::build(const GLProgram* program) {
    return build(program->getSemanticBinding());
}

void vg::ShaderInterface::use() {
    glBindVertexArray(m_vao);
}

void vg::ShaderInterface::unuse() {
    glBindVertexArray(0);
}
