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

bool drawTriangle = true;

GLenum polygonMode = GL_FILL;

void renderer::swapShape()
{
    drawTriangle = !drawTriangle;
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

    float triH = 1.0f;
    float triW = 1.0f;
    glm::vec3 triColor(1.0f, 1.0f, 1.0f);
    glm::vec3 triPos(0.0f, 0.0f, 0.0f);
    triangle = new Triangle(triH, triW, triPos, triColor, textureShader, texture);

    float recH = 1.0f;
    float recW = 1.0f;
    glm::vec3 recColor(1.0f, 0.7f, 0.0f);
    glm::vec3 recPos(0.0f, 0.0f, 0.0f);
    rectangle = new Quad(recH, recW, recPos, recColor, textureShader, texture);
}

void renderer::render()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (drawTriangle)
    {
        triangle->draw();
    }
    else
    {
        rectangle->draw();
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