#pragma once
#include "shape.h"

class Quad : public Shape
{
  public:
    Quad(const float *vertices, size_t vertexByteSize, unsigned int *indices, size_t indexByteSize, Shader *shader,
         Texture *texture);
    ~Quad();
    void draw() override;

  private:
    unsigned int *indices_;
    unsigned int ebo_;
    size_t indexByteSize_;
};