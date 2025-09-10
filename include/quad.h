#pragma once
#include "shape.h"
#include <cstddef>
#include <glm/glm.hpp>

class Quad : public Shape
{
  public:
    Quad(float height, float width, glm::vec3 pos, Shader *shader, Texture *texture);
    ~Quad();
    void draw() override;

  private:
    unsigned int *indices_;
    unsigned int ebo_;
    float height_;
    float width_;
    size_t indexByteSize_;
};