#include "triangle.h"

Triangle::Triangle(const float *vertices, size_t vertexByteSize, Shader *shader, Texture *texture)
    : Shape(vertices, vertexByteSize, shader, texture) {}

void Triangle::draw()
{
    shader_->use();
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}