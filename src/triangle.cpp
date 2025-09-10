#include "triangle.h"

Triangle::Triangle(float height, float width, glm::vec3 pos, Shader *shader, Texture *texture)
    : Shape(shader, texture)
{
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    float vertices[] = {
        pos.x - halfWidth, pos.y - halfHeight, pos.z, 0.0f, 0.0f,
        pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f, 0.0f,
        pos.x,             pos.y + halfHeight, pos.z, 0.5f, 1.0f,
    };

    init(vertices, sizeof(vertices));

    shader_->use();
    shader_->setInt("tex0", 0);
}

void Triangle::draw()
{
    texture_->use();
    shader_->use();

    glm::vec3 translate(-0.33f, -0.33f, 0.0f);
    glm::vec3 rotate(0.0f, 1.0f, 0.0f);
    transform(translate, rotate);

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}