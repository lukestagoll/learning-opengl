#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "camera.h"
#include "constants.h"
#include "cube.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <SDL3/SDL.h>

GLint success;
GLchar infoLog[512];

Shader *rectangleShader = nullptr;
Texture *rectangleTexture = nullptr;
Cube *cube = nullptr;

int scene = 0;

GLenum polygonMode = GL_FILL;


// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(3.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 3.0f),
    glm::vec3(1.0f, 3.0f, 1.0f),
    glm::vec3(-3.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f, -3.0f),
    glm::vec3(-1.0f, -3.0f, -1.0f),
};

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
    cube = new Cube(recSize, rectangleShader, rectangleTexture);
    rectangleShader->setProjection(glm::radians(45.0f));

    glEnable(GL_DEPTH_TEST);
}

void renderer::render(Camera *camera)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rectangleShader->use();
    rectangleTexture->use();
    // glm::vec3 translate(0.0f, 0.0f, -3.0f);

    glm::mat4 projection = camera->getProjection((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    rectangleShader->setProjection(projection);

    glm::mat4 view = camera->getViewMatrix(); 
    rectangleShader->setView(view);

    cube->bind();
    for (int i = 0; i < 7; i++)
    {
        rectangleShader->setModel(cubePositions[i], glm::radians(20.0f * i));
        cube->draw();
    }

    glBindVertexArray(0);
}

void renderer::cleanup()
{
    delete rectangleShader;
    delete rectangleTexture;
    delete cube;
}