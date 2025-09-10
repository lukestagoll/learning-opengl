#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

enum class ShaderType
{
    Vertex,
    Fragment,
    Program,
};

class Shader
{
  public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    GLuint getID() const
    {
        return id_;
    }

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setProjection(GLfloat degrees);
    void setView(glm::vec3 translate);
    void setModel(GLfloat degrees);

  private:
    GLuint id_;
    void checkCompileErrors(GLuint shader, ShaderType type);
    std::string loadFile(const char *filePath);
    GLuint compileShader(ShaderType type, const char *shaderSourceCode);
    void createProgram(GLuint vertexShader, GLuint fragmentShader);
};