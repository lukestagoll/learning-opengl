#pragma once

#include "shader.h"
#include "texture.h"

#include <cstddef>

class Shape
{
  public:
    virtual void draw() = 0;

  protected:
    Shape(const float *vertices, size_t vertexByteSize, Shader *shader, Texture *texture)
        : vertices_(vertices), vertexByteSize_(vertexByteSize), shader_(shader), texture_(texture)
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
        glBufferData(GL_ARRAY_BUFFER, vertexByteSize_, vertices_, GL_STATIC_DRAW);

        vertexStride_ = texture == nullptr ? 6 : 8;

        // ------------------------------------- //
        // ----- Linking Vertex Attributes ----- //
        // ------------------------------------- //

        /*
         *  Our vertex buffer data is an array, so we need to tell OpenGL how to interpret the data for use in the
         * vertex shader
         *
         *            ┌───────────────────────────────────┬───────────────────────────────────┬─────┐
         *            │             Vertex 1              │             Vertex 2              │ ... │
         *            ├─────────────────┬─────────────────┼─────────────────┬─────────────────┼─────┤
         *            │     Position    │      Color      │     Position    │      Color      │     │
         *            ├─────┬─────┬─────┼─────┬─────┬─────┼─────┬─────┬─────┼─────┬─────┬─────┼─────┤
         *            │  X  │  Y  │  Z  │  R  │  G  │  B  │  X  │  Y  │  Z  │  R  │  G  │  B  │ ... │
         *            └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
         *      BYTE: 0     4     8     12    16    20    24    28    32    36    40    44    48    72
         *
         *  POSITION: ──────────── STRIDE: 24 ───────────➤
         *            ─ OFFSET: 0
         *
         *     COLOR:                   ──────────── STRIDE: 24 ───────────➤
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride_ * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride_ * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    ~Shape()
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
    }

    const float *vertices_;
    size_t vertexByteSize_;
    size_t vertexStride_;
    unsigned int vao_;
    unsigned int vbo_;
    Shader *shader_ = nullptr;
    Texture *texture_ = nullptr;
};