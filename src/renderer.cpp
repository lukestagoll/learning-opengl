#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "quad.h"
#include "triangle.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

GLint success;
GLchar infoLog[512];

Shader *triangleShader = nullptr;
Shader *rectangleShader = nullptr;
Texture *triangleTexture = nullptr;
Texture *rectangleTexture = nullptr;
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
    triangleShader = new Shader("assets/shaders/vert.vert", "assets/shaders/texture.frag");
    rectangleShader = new Shader("assets/shaders/vert.vert", "assets/shaders/texture.frag");
    triangleTexture = new Texture("lamp_1_emission", GL_TEXTURE0);
    rectangleTexture = new Texture("crate_1", GL_TEXTURE0);

    float triH = 0.75f;
    float triW = 0.75f;
    glm::vec3 triPos(0.0f, 0.0f, 0.0f);
    triangle = new Triangle(triH, triW, triPos, triangleShader, triangleTexture);

    float recH = 0.75f;
    float recW = 0.75f;
    glm::vec3 recPos(0.0f, 0.0f, 0.0f);
    rectangle = new Quad(recH, recW, recPos, rectangleShader, rectangleTexture);
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
    delete triangleShader;
    delete rectangleShader;
    delete triangleTexture;
    delete rectangleTexture;
    delete triangle;
    delete rectangle;
}