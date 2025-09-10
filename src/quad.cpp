#include "quad.h"
#include <iostream>

Quad::Quad(float height, float width, glm::vec3 pos, Shader *shader, Texture *texture)
    : Shape(shader, texture), height_(height), width_(width)
{
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    std::cout << pos.x + halfWidth << std::endl;

    /*
     *  If we want to draw a rectangle we can do so by drawing two triangles using the follwoing verticies:
     *      GLfloat vertices[] = {
     *         // first triangle
     *          0.5f,  0.5f, 0.0f,  // top right
     *          0.5f, -0.5f, 0.0f,  // bottom right
     *         -0.5f,  0.5f, 0.0f,  // top left
     *         // second triangle
     *          0.5f, -0.5f, 0.0f,  // bottom right
     *         -0.5f, -0.5f, 0.0f,  // bottom left
     *         -0.5f,  0.5f, 0.0f   // top left
     *      };
     *
     *  The issue here is we have defined top left and bottom right twice.
     *  This creates overhead that gets worse the more complex your model is.
     *
     *  Instead of doing this, we can store unique vertices, then tell OpenGL the order we want the
     *  vertices to be drawn.
     *  This is done using an array of indices.
     */
    float vertices[] = {
        pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f, 1.0f,
        pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f, 0.0f,
        pos.x - halfWidth, pos.y - halfHeight, pos.z, 0.0f, 0.0f,
        pos.x - halfWidth, pos.y + halfHeight, pos.z, 0.0f, 1.0f,
    };
    init(vertices, sizeof(vertices));

    int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // ---------------------------------- //
    // ----- Element Buffer Objects ----- //
    // ---------------------------------- //

    // An Eement Buffer Object (EBO) stores indices that OpenGL uses to determine what vertices to draw.
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader_->use();
    shader_->setInt("tex0", 0);
}

Quad::~Quad()
{
    glDeleteBuffers(1, &ebo_);
}

void Quad::draw()
{
    texture_->use();
    shader_->use();
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}