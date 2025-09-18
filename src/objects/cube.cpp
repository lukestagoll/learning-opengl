#include "cube.h"


#include <glm/gtc/matrix_transform.hpp>

Cube::Cube(glm::vec3 size, Shader *shader, Texture *texture)
    : size_(size), shader_(shader), texture_(texture)
{
    float halfWidth = size.x / 2;
    float halfHeight = size.y / 2;
    float halfDepth = size.z / 2;

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
        // front (+Z)
        // position                        // normals        // texture coords
        halfWidth,  halfHeight, halfDepth, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        halfWidth, -halfHeight, halfDepth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -halfWidth,-halfHeight, halfDepth, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -halfWidth, halfHeight, halfDepth, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // back (-Z)
        halfWidth,  halfHeight,-halfDepth, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        halfWidth, -halfHeight,-halfDepth, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -halfWidth,-halfHeight,-halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        -halfWidth, halfHeight,-halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

        // back (+X)
        halfWidth,  halfHeight,-halfDepth, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        halfWidth, -halfHeight,-halfDepth, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        halfWidth, -halfHeight, halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        halfWidth,  halfHeight, halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // back (-X)
        -halfWidth, halfHeight, halfDepth, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -halfWidth,-halfHeight, halfDepth, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -halfWidth,-halfHeight,-halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -halfWidth, halfHeight,-halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // back (+Y)
        halfWidth,  halfHeight,-halfDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -halfWidth, halfHeight,-halfDepth, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -halfWidth, halfHeight, halfDepth, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        halfWidth,  halfHeight, halfDepth, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        // back (-Y)
        halfWidth, -halfHeight, halfDepth, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -halfWidth,-halfHeight, halfDepth, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -halfWidth,-halfHeight,-halfDepth, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        halfWidth, -halfHeight,-halfDepth, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    };

    int indices[] = {
        // front
        0,  1,  3,
        1,  2,  3,
        
        // back
        4,  5,  7,
        5,  6,  7,

        // right
        8,  9, 11,
        9, 10, 11,

        // left
        12, 13, 15,
        13, 14, 15,

        // top
        16, 17, 19,
        17, 18, 19,
    
        // bottom
        20, 21, 23,
        21, 22, 23
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Cube::vertexStride * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Cube::vertexStride * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Cube::vertexStride * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader_->use();
    shader_->setInt("tex0", 0);
}

Cube::~Cube()
{
    glDeleteBuffers(1, &ebo_);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void Cube::bind()
{
    glBindVertexArray(vao_);
}

void Cube::draw()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}