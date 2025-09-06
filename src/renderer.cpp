#include "renderer.h"
#include "shader.h"

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
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, // top left
};

GLuint rectangleIndices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

GLint success;
GLchar infoLog[512];

GLuint triangleVAO, triangleVBO;
GLuint rectangleVAO, rectangleVBO, rectangleEBO;
Shader *shader = nullptr;

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
    shader = new Shader("shaders/vert.glsl", "shaders/frag.glsl");

    // ------------------------ //
    // ----- Vertex Input ----- //
    // ------------------------ //

    /*
     *  For the GPU to be able to draw anything, it needs vertex data.
     *  The GPU holds vertex data in a Vertex Buffer Object (VBO).
     *  We copy vertex data from our CPU-side array to a VBO, then send the VBO to the GPU for storage.
     *
     *  We also need to tell OpenGL how to interpret the data in our VBOs.
     *  This is done with attribute pointers - more on this below (Linking Vertex Attributes)
     *
     *  We also need to be able to store this attribute metadata - otherwise, when using multiple objects
     *  with different vertex formats we would need to reconfigure everything before drawing.
     *
     *  These attribute configurations are stored in Vertex Array Objects (VAOs)
     *
     *  Order of operations: bind VAO, bind & set VBO, configure vertex attributes
     */
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);

    // VBO is of type GL_ARRAY_BUFFER, so we bind it to the GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

    /*
     *  Since we bound VBO to GL_ARRAY_BUFFER, any buffer calls we make to GL_ARRAY_BUFFER uses VBO
     *  glBufferData copies user-defined data into the target buffer (GL_ARRAY_BUFFER)
     *  The fourth param of glBufferData specifies how the GPU should manage the data:
     *      - GL_STREAM_DRAW:  data set once & used a few times at most
     *      - GL_STATIC_DRAW:  data set once & used many times
     *      - GL_DYNAMIC_DRAW: data changes a lot & used many times
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // ------------------------------------- //
    // ----- Linking Vertex Attributes ----- //
    // ------------------------------------- //

    /*
     *  Our vertex buffer data is an array, so we need to tell OpenGL how to interpret the data for use in the vertex
     *  shader
     *
     *            ┌───────────────────────────────────┬───────────────────────────────────┬─────┐
     *            │              Vertex 1             │              Vertex 2             │ ... │
     *            ├─────┬─────┬─────┬─────┬─────┬─────┼─────┬─────┬─────┬─────┬─────┬─────┼─────┤
     *            │  X  │  Y  │  Z  │  R  │  G  │  B  │  X  │  Y  │  Z  │  R  │  G  │  B  │ ... │
     *            └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
     *  BYTE:     0     4     8     12    16    20    24    28    32    36    40    44    48    72
     *  POSITION: ──────────── STRIDE: 24 ───────────➤
     *            ─ OFFSET: 0
     *     COLOR:                   ──────────── STRIDE: 24 ───────────➤
     *            ── OFFSET: 12 ──➤
     *
     *  3 floats per vertex
     *
     *  Use `glVertexAttribPointer` to tell OpenGL how to interpret the vertex data.
     *  `glVertexAttribPointer` params:c
     *      1. index (GLuint) - In the vertex shader we specified the location position attr as `layout (location = 0)`,
     *         this is where we want to pass the vertex data, so we set index to 0.
     *      2. size (GLuint - either 1, 2, 3, or 4)
     *      3. type (GL_FLOAT in this example)
     *      4. normalized (GLboolean - specifies whether or not the data needs to be normalised)
     *      5. stride - byte offset of consecutive vertex attributes.
     *      6. pointer - offset where the position data starts in the array.
     */

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ---------------------------------- //
    // ----- Element Buffer Objects ----- //
    // ---------------------------------- //

    /*
     *  An Eement Buffer Object (EBO) stores indices that OpenGL uses to determine what vertices to draw.
     *
     */

    glGenVertexArrays(1, &rectangleVAO);
    glGenBuffers(1, &rectangleVBO);
    glGenBuffers(1, &rectangleEBO);

    glBindVertexArray(rectangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void renderer::render()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    if (drawTriangle)
    {
        glBindVertexArray(triangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    else
    {
        glBindVertexArray(rectangleVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void renderer::cleanup()
{
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);

    glDeleteVertexArrays(1, &rectangleVAO);
    glDeleteBuffers(1, &rectangleVBO);
    glDeleteBuffers(1, &rectangleEBO);

    delete shader;
}