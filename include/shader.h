#pragma once

#include <glad/glad.h>

#include <cstdint>
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

    uint32_t getID() const
    {
        return id_;
    }

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

  private:
    uint32_t id_;
    void checkCompileErrors(uint32_t shader, ShaderType type);
    std::string loadFile(const char *filePath);
    uint32_t compileShader(ShaderType type, const char *shaderSourceCode);
    void createProgram(uint32_t vertexShader, uint32_t fragmentShader);
};