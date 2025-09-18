#pragma once

#include "cube.h"
#include "texture.h"
#include "shader.h"

#include <glm/glm.hpp>

#include <cstddef>

class Cube
{
  public:
    Cube(glm::vec3 size, Shader *shader, Texture *diff, Texture *spec);
    ~Cube();
    void bind();
    void draw();
    void transform(glm::vec3 translate, glm::vec3 rotate);

  private:
    glm::vec3 size_;

    static constexpr size_t vertexStride = 8; // 3 pos vertices, 3 normals, 2 tex coord vertices
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;

    Shader *shader_ = nullptr;
    Texture *diff_ = nullptr;
    Texture *spec_ = nullptr;
};