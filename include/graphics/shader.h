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

    void setVec3(const std::string &name, glm::vec3 value);
    void setMat4(const std::string &name, const glm::mat4 &mat);

    void setProjection(glm::mat4 projection);
    void setView(glm::mat4 view);
    void setModel(glm::vec3 pos, GLfloat rot);

  private:
    GLuint id_;
    void checkCompileErrors(GLuint shader, ShaderType type);
    std::string loadFile(const char *filePath);
    GLuint compileShader(ShaderType type, const char *shaderSourceCode);
    void createProgram(GLuint vertexShader, GLuint fragmentShader);
};