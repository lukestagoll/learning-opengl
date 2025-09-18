#include "texture.h"

#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <string>

Texture::Texture(const char *name, int texUnit)
{
    textureUnit_ = texUnit;
    glGenTextures(1, &id_);
    glActiveTexture(textureUnit_);
    glBindTexture(GL_TEXTURE_2D, id_);
    setTextureParams();

    std::string filePath = "assets/textures/" + std::string(name) + ".png";
    unsigned char *data = loadImage(filePath.c_str());
    if (data)
    {
        compileTexture(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

void Texture::use()
{
    glActiveTexture(textureUnit_);
    glBindTexture(GL_TEXTURE_2D, id_);
}

unsigned char *Texture::loadImage(const char *filePath)
{
    return stbi_load(filePath, &width_, &height_, &nrChannels_, 0);
}

void Texture::compileTexture(unsigned char *imageData)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setTextureParams()
{
    // texture wrapping parameters - GL_REPEAT (default), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // mipmap filtering on the min filter, not the mag filter
    // GL_LINEAR (smooth), GL_NEAREST (sharp)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}