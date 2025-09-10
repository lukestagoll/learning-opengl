#pragma once

#include "shader.h"
#include "texture.h"

#include <cstddef>

class Shape
{
  public:
    virtual void draw() = 0;

  protected:
    Shape(Shader *shader, Texture *texture) : shader_(shader), texture_(texture) {}
    ~Shape();

    void init(const float *vertices, size_t vertexByteSize);

    static constexpr size_t vertexStride = 5; // 3 pos vertices, 2 tex coord vertices 
    unsigned int vao_;
    unsigned int vbo_;
    Shader *shader_ = nullptr;
    Texture *texture_ = nullptr;
};