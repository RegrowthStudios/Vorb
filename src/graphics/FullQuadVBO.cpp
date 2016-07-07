#include "stdafx.h"
#include "graphics/FullQuadVBO.h"

#ifndef VORB_USING_PCH
#include <GL/glew.h>
#endif // !VORB_USING_PCH

void vg::FullQuadVBO::init(i32 attrLocation /*= 0*/) {
    glGenBuffers(2, m_buffers);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    ui32 inds[6] = { 0, 1, 3, 0, 3, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    f32 points[8] = { -1, -1, 1, -1, -1, 1, 1, 1 };
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glVertexAttribPointer(attrLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBindVertexArray(0);
}

void vg::FullQuadVBO::dispose() {
    if (m_vao) {
        glDeleteBuffers(2, m_buffers);
        m_buffers[0] = 0;
        m_buffers[1] = 0;

        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0; ///< So we know we aren't initialized.
    }
}

void vg::FullQuadVBO::draw() {
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}
