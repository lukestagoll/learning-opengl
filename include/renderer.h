#pragma once
#include <glad/glad.h>

#include "camera.h"

namespace renderer
{
void render(Camera *camera);
void init();
void nextScene();
void swapPolygonMode();
void cleanup();
}; // namespace renderer