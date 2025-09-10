#pragma once
#include "shape.h"

class Triangle : public Shape
{
  public:
    Triangle(const float *vertices, size_t vertexByteSize, Shader *shader, Texture *texture);
    ~Triangle() {}
    void draw() override;
};