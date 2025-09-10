#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "cube.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

GLint success;
GLchar infoLog[512];

Shader *rectangleShader = nullptr;
Texture *rectangleTexture = nullptr;
Cube *cube = nullptr;

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
    rectangleShader = new Shader("assets/shaders/vert.vert", "assets/shaders/texture.frag");
    rectangleTexture = new Texture("crate_1", GL_TEXTURE0);

    glm::vec3 recSize(1.0f, 1.0f, 1.0f);
    glm::vec3 recPos(0.0f, 0.0f, 0.0f);
    cube = new Cube(recSize, recPos, rectangleShader, rectangleTexture);

    glEnable(GL_DEPTH_TEST);
}

void renderer::render()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube->draw();

    glBindVertexArray(0);
}

void renderer::cleanup()
{
    delete rectangleShader;
    delete rectangleTexture;
    delete cube;
}