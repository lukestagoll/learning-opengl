#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "quad.h"
#include "triangle.h"

#include <glad/glad.h>

/*
 *  vertex data for a triangle in normalised device coordinates (NDC) (between -1 and 1).
 *  3 sets of x,y,z position values and r,g,b color values.
 */
GLfloat triangleVertices[] = {
    // positions        // colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
};

/*
 *  If we want to draw a rectangle we can do so by drawing two triangles using the follwoing verticies:
 *      GLfloat vertices[] = {
 *         // first triangle
 *          0.5f,  0.5f, 0.0f,  // top right
 *          0.5f, -0.5f, 0.0f,  // bottom right
 *         -0.5f,  0.5f, 0.0f,  // top left
 *         // second triangle
 *          0.5f, -0.5f, 0.0f,  // bottom right
 *         -0.5f, -0.5f, 0.0f,  // bottom left
 *         -0.5f,  0.5f, 0.0f   // top left
 *      };
 *
 *  The issue here is we have defined top left and bottom right twice.
 *  This creates overhead that gets worse the more complex your model is.
 *
 *  Instead of doing this, we can store unique vertices, then tell OpenGL the order we want the
 *  vertices to be drawn.
 *  This is done using an array of indices.
 */
GLfloat rectangleVertices[] = {
    // positions        // colors         // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.7f, 0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 1.0f, 0.7f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 0.7f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 0.7f, 0.0f, 0.0f, 1.0f, // top left
};

GLuint rectangleIndices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

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

    triangle = new Triangle(triangleVertices, sizeof(triangleVertices), colorShader, nullptr);
    rectangle = new Quad(rectangleVertices, sizeof(rectangleVertices), rectangleIndices, sizeof(rectangleIndices),
                         textureShader, texture);
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