#include "triangle.h"

Triangle::Triangle(float height, float width, glm::vec3 pos, glm::vec3 color, Shader *shader, Texture *texture)
    : Shape(shader, texture)
{
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    float vertices[] = {
        pos.x - halfWidth, pos.y - halfHeight, pos.z, color.x, color.y, color.z, 0.0f, 0.0f,
        pos.x + halfWidth, pos.y - halfHeight, pos.z, color.x, color.y, color.z, 1.0f, 0.0f,
        pos.x,             pos.y + halfHeight, pos.z, color.x, color.y, color.z, 0.5f, 1.0f,
    };

    init(vertices, sizeof(vertices));

    shader_->use();
    shader_->setInt("tex0", 0);
}

void Triangle::draw()
{
    shader_->use();
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}