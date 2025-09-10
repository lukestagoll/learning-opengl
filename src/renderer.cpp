#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "quad.h"
#include "triangle.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

GLint success;
GLchar infoLog[512];

Shader *colorShader = nullptr;
Shader *textureShader = nullptr;
Texture *texture = nullptr;
Triangle *triangle = nullptr;
Quad *rectangle = nullptr;

int scene = 0;

GLenum polygonMode = GL_FILL;

void renderer::nextScene()
{
    scene = (scene + 1) % 3;
}

void renderer::swapPolygonMode()
{
    polygonMode = polygonMode == GL_FILL ? GL_LINE : GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

void renderer::init()
{
    colorShader = new Shader("assets/shaders/vert.vert", "assets/shaders/color.frag");
    textureShader = new Shader("assets/shaders/vert.vert", "assets/shaders/texture.frag");
    texture = new Texture("crate_1", GL_TEXTURE0);

    float triH = 0.75f;
    float triW = 0.75f;
    glm::vec3 triPos(-0.33f, -0.33f, 0.0f);
    triangle = new Triangle(triH, triW, triPos, textureShader, texture);

    float recH = 0.75f;
    float recW = 0.75f;
    glm::vec3 recPos(0.33f, 0.33f, 0.0f);
    rectangle = new Quad(recH, recW, recPos, textureShader, texture);
}

void renderer::render()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (scene)
    {
    case 0:
        triangle->draw();
        break;
    case 1:
        rectangle->draw();
        break;
    case 2:
        triangle->draw();
        rectangle->draw();
        break;
    }

    glBindVertexArray(0);
}

void renderer::cleanup()
{
    delete colorShader;
    delete textureShader;
    delete texture;
    delete triangle;
    delete rectangle;
}