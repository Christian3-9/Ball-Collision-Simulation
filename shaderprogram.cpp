#include "shaderprogram.hpp"
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram()
{

}

void ShaderProgram::set_shader_filepaths(std::string vertexShaderFilepath,
                                         std::string fragmentShaderFilepath)
{
    this->vertexShaderFilepath=vertexShaderFilepath;
    this->fragmentShaderFilepath=fragmentShaderFilepath;
}

void ShaderProgram::set_shader_code(std::string vertexShaderCode, std::string fragmentShaderCode)
{
    this->vertexShaderCode = vertexShaderCode;
    this->fragmentShaderCode = fragmentShaderCode;
}

std::string ShaderProgram::read_shader_code(const std::string& shaderFilepath)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(shaderFilepath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::stringstream errorstream;
        errorstream << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        std::string errorMessage = errorstream.str();
        throw errorMessage;
    }

    return shaderCode;
}

void ShaderProgram::create()
{
    initializeOpenGLFunctions();

    if(vertexShaderCode.empty())
    {
        vertexShaderCode = read_shader_code(vertexShaderFilepath);
    }

    if(fragmentShaderCode.empty())
    {
        fragmentShaderCode = read_shader_code(fragmentShaderFilepath);
    }

    const char* vShaderCode = vertexShaderCode.c_str();
    const char* fShaderCode = fragmentShaderCode.c_str();

    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);
    glCompileShader(vertexShaderID);
    checkShaderCompileErrors(vertexShaderID, "VERTEX");

    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShaderID);

    try
    {
        checkShaderCompileErrors(fragmentShaderID, "FRAGMENT");
    }
    catch (std::runtime_error e)
    {
        glDeleteShader(fragmentShaderID);
        throw;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    try
    {
        checkProgramLinkingErrors(programID);
    }
    catch (std::runtime_error e)
    {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        glDeleteProgram(programID);
        throw;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void ShaderProgram::destroy()
{
    glDeleteProgram(programID);
    programID=0;
}

void ShaderProgram::use()
{
    glUseProgram(programID);
}

void ShaderProgram::setUniform(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& mat4)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat4));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vec3)
{
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(vec3));
}

void ShaderProgram::setUniform(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::checkShaderCompileErrors(unsigned int shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    std::stringstream errorstream;
    std::string errors;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        errorstream << type << "shader compilation error." << std::endl << infoLog << std::endl;
        errors = errorstream.str();
        throw std::runtime_error(errors);
    }
}

void ShaderProgram::checkProgramLinkingErrors(unsigned int shader)
{
    int success;
    char infoLog[1024];
    std::stringstream errorstream;
    std::string errors;

    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        errorstream << "Program linking error." << std::endl << infoLog << std::endl;
        errors = errorstream.str();
        throw std::runtime_error(errors);
    }
}
