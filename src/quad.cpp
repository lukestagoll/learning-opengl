#include "quad.h"

Quad::Quad(const float *vertices, size_t vertexByteSize, unsigned int *indices, size_t indexByteSize, Shader *shader,
           Texture *texture)
    : Shape(vertices, vertexByteSize, shader, texture), indices_(indices), indexByteSize_(indexByteSize)
{
    // ---------------------------------- //
    // ----- Element Buffer Objects ----- //
    // ---------------------------------- //

    // An Eement Buffer Object (EBO) stores indices that OpenGL uses to determine what vertices to draw.
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize_, indices_, GL_STATIC_DRAW);

    /*
        *  Our vertex buffer data is an array, so we need to tell OpenGL how to interpret the data for use in the
        *  vertex shader
        *
        *            ┌───────────────────────────────────────────────┬───────────────────────────────────────────────┬─────┐
        *            │                   Vertex 1                    │                   Vertex 2                    │ ... │
        *            ├─────────────────┬─────────────────┬───────────┼─────────────────┬─────────────────┬───────────┼─────┤
        *            │     Position    │      Color      │  Texture  │     Position    │      Color      │  Texture  │     │
        *            ├─────┬─────┬─────┼─────┬─────┬─────┼─────┬─────┼─────┬─────┬─────┼─────┬─────┬─────┼─────┬─────┼─────┤
        *            │  X  │  Y  │  Z  │  R  │  G  │  B  │  S  │  T  │  X  │  Y  │  Z  │  R  │  G  │  B  │  S  │  T  │ ... │
        *            └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
        *      BYTE: 0     4     8     12    16    20    24    28    32    36    40    44    48    52    56    60    64
        *
        *  POSITION: ────────────────── STRIDE: 32 ─────────────────➤
        *            ─ OFFSET: 0
        *
        *     COLOR:                   ────────────────── STRIDE: 32 ─────────────────➤
        *            ── OFFSET: 12 ──➤
        *
        *    TEXTURE:                                    ────────────────── STRIDE: 32 ─────────────────➤
        *            ─────────── OFFSET: 24 ───────────➤
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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader_->use();
    shader_->setInt("tex0", 0);
}

Quad::~Quad()
{
    glDeleteBuffers(1, &ebo_);
}

void Quad::draw()
{
    texture_->use();
    shader_->use();
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}