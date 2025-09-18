#include "renderer.h"
#include "glm/fwd.hpp"
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

Shader *lightingShader = nullptr;
Shader *lightsourceShader = nullptr;
Texture *cubeDiffTexture = nullptr;
Texture *cubeSpecTexture = nullptr;
Texture *lightsourceTexture = nullptr;
Cube *cube = nullptr;
Cube *lightsource = nullptr;

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
    lightingShader = new Shader("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");
    lightsourceShader = new Shader("assets/shaders/lightsource.vert", "assets/shaders/lightsource.frag");

    cubeDiffTexture = new Texture("crate_1", GL_TEXTURE0);
    cubeSpecTexture = new Texture("crate_1_spec", GL_TEXTURE1);
    lightsourceTexture = new Texture("lamp_1_emission", GL_TEXTURE0);

    glm::vec3 lightsourceObjSize(1.0f, 1.0f, 1.0f);
    lightsource = new Cube(lightsourceObjSize, lightsourceShader, lightsourceTexture, lightsourceTexture);

    glm::vec3 recSize(1.0f, 1.0f, 1.0f);
    cube = new Cube(recSize, lightingShader, cubeDiffTexture, cubeSpecTexture);

    glEnable(GL_DEPTH_TEST);
}

void renderer::render(Camera *camera)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader->use();

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
    glm::vec3 lightDiffuse = lightColor * glm::vec3(0.5f);
    glm::vec3 lightAmbient = lightColor * glm::vec3(0.3f);
    lightingShader->setVec3("light.position", lightPos);
    lightingShader->setVec3("light.ambient", lightAmbient);
    lightingShader->setVec3("light.diffuse", lightDiffuse);
    lightingShader->setVec3("light.specular", lightSpecular);

    glm::vec3 specular(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;
    lightingShader->setFloat("material.shininess", shininess);

    // view/projection transformations
    glm::mat4 projection = camera->getProjection((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::vec3 viewPos = camera->getPosition();
    lightingShader->setVec3("viewPos", viewPos);
    lightingShader->setProjection(projection);
    lightingShader->setView(view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader->setMat4("model", model);

    // render the cube
    cubeDiffTexture->use();
    cubeSpecTexture->use();
    cube->bind();
    cube->draw();

    // for (int i = 0; i < 7; i++)
    // {
    //     lightingShader->setModel(cubePositions[i], glm::radians(20.0f * i));
    //     cube->draw();
    // }

    // render the light object
    lightsourceShader->use();
    lightsourceTexture->use();
    lightsourceShader->setProjection(projection);
    lightsourceShader->setView(view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightsourceShader->setMat4("model", model);
    lightsource->bind();
    lightsource->draw();

    glBindVertexArray(0);
}

void renderer::cleanup()
{
    delete lightingShader;
    delete cubeDiffTexture;
    delete lightsourceTexture;
    delete cube;
}