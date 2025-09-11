#include "shader.h"
#include "constants.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include <SDL3/SDL.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexCode = loadFile(vertexPath);
    std::string fragmentCode = loadFile(fragmentPath);

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    uint32_t vertex = compileShader(ShaderType::Vertex, vShaderCode);
    uint32_t fragment = compileShader(ShaderType::Fragment, fShaderCode);

    // shader Program
    createProgram(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(id_);
}

void Shader::use()
{
    glUseProgram(id_);
}

void Shader::setProjection(GLfloat degrees)
{
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(degrees, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    int projectionLoc = glGetUniformLocation(id_, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::setView(glm::vec3 translate)
{
    glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    float radius = 10.0f;
    float time = SDL_GetTicks() / 1000.0f;
    float camX = static_cast<float>(sin(time) * radius);
    float camZ = static_cast<float>(cos(time) * radius);
    view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    int viewLoc = glGetUniformLocation(id_, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void Shader::setModel(glm::vec3 pos, GLfloat rot)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, rot, pos);
    int modelLoc = glGetUniformLocation(id_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

/* Private Functions */

std::string Shader::loadFile(const char *filePath)
{
    std::string shaderCode;
    std::ifstream file;

    // ensure ifstream throws exceptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream stream;

        file.open(filePath);
        stream << file.rdbuf();
        file.close();
        shaderCode = stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FAILED_TO_READ_FILE" << std::endl;
    }

    return shaderCode;
}

uint32_t Shader::compileShader(ShaderType type, const char *shaderSourceCode)
{

    uint32_t shader = glCreateShader(type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &shaderSourceCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, type);

    return shader;
}

void Shader::createProgram(uint32_t vertexShader, uint32_t fragmentShader)
{
    id_ = glCreateProgram();
    glAttachShader(id_, vertexShader);
    glAttachShader(id_, fragmentShader);
    glLinkProgram(id_);
    checkCompileErrors(id_, ShaderType::Program);
}

void Shader::checkCompileErrors(uint32_t shader, ShaderType type)
{
    int success;
    char infoLog[1024];
    std::string shaderName;

    switch (type)
    {
    case ShaderType::Vertex:
        shaderName = "Vertex";
        break;
    case ShaderType::Fragment:
        shaderName = "Fragment";
        break;
    case ShaderType::Program:
        shaderName = "Program";
        break;
    }

    if (type != ShaderType::Program)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderName << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << shaderName << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}