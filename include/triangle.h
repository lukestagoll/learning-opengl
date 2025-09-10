#pragma once
#include "shape.h"
#include <glm/glm.hpp>

class Triangle : public Shape
{
  public:
    Triangle(float height, float width, glm::vec3 pos, glm::vec3 color, Shader *shader, Texture *texture);
    ~Triangle() {}
    void draw() override;
};