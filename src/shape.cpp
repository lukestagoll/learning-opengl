#include "shape.h"

Shape::~Shape()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void Shape::init(const float *vertices, size_t vertexByteSize)
{
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
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    // VBO is of type GL_ARRAY_BUFFER, so we bind it to the GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertexByteSize, vertices, GL_STATIC_DRAW);

    // ------------------------------------- //
    // ----- Linking Vertex Attributes ----- //
    // ------------------------------------- //

    /*
     *  Our vertex buffer data is an array, so we need to tell OpenGL how to interpret the data for use in the
     *  vertex shader
     *
     *            ┌─────────────────────────────┬─────────────────────────────┬─────┐
     *            │           Vertex 1          │           Vertex 2          │ ... │
     *            ├─────────────────┬───────────┼─────────────────┬───────────┼─────┤
     *            │     Position    │  Texture  │     Position    │  Texture  │     │
     *            ├─────┬─────┬─────┼─────┬─────┼─────┬─────┬─────┼─────┬─────┼─────┤
     *            │  X  │  Y  │  Z  │  S  │  T  │  X  │  Y  │  Z  │  S  │  T  │ ... │
     *            └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
     *      BYTE: 0     4     8     12    16    20    24    28    32    36    40
     *
     *  POSITION: ───────── STRIDE: 20 ────────➤
     *            ─ OFFSET: 0
     *
     *   TEXTURE:                   ───────── STRIDE: 20 ────────➤
     *            ── OFFSET: 12 ──➤
     *
     *  3 floats per vertex
     *
     *  Use `glVertexAttribPointer` to tell OpenGL how to interpret the vertex data.
     *  `glVertexAttribPointer` params:c
     *      1. index (GLuint) - In the vertex shader we specified the location position attr as `layout (location = 0)`,
     *                          this is where we want to pass the vertex data, so we set index to 0.
     *      2. size (GLuint - either 1, 2, 3, or 4)
     *      3. type (GL_FLOAT in this example)
     *      4. normalized (GLboolean - specifies whether or not the data needs to be normalised)
     *      5. stride - byte offset of consecutive vertex attributes.
     *      6. pointer - offset where the position data starts in the array.
     */

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Shape::vertexStride * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Shape::vertexStride * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}