#pragma once
#include <glad/glad.h>

namespace renderer
{
    void render();
    void init();
    void updateShapeColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void swapShape();
    void swapPolygonMode();
    void cleanup();
};