#pragma once

#include <glad/glad.h>

class Texture
{
  public:
    Texture(const char *name, int texUnit);

    void use();

    GLuint getID() const
    {
        return id_;
    }

  private:
    GLuint id_;
    int textureUnit_;
    int width_, height_, nrChannels_;
    unsigned char *loadImage(const char *filePath);
    void compileTexture(unsigned char *imageData);
    void setTextureParams();
};