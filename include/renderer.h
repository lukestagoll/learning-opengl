#pragma once
#include <glad/glad.h>

namespace renderer
{
    void render();
    void init();
    void nextScene();
    void swapPolygonMode();
    void cleanup();
};