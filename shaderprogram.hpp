#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <glm/glm.hpp>
#include <string>
#include <QOpenGLExtraFunctions>

class ShaderProgram:protected QOpenGLExtraFunctions
{
public:
    ShaderProgram();

    void set_shader_filepaths(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
    void set_shader_code(std::string vertexShaderCode, std::string fragmentShaderCode);

    void create();
    void destroy();

    void use();
    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::mat4& mat4);
    void setUniform(const std::string& name, const glm::vec3& vec3);

protected:
    std::string read_shader_code(const std::string& shaderFilepath);
    void checkShaderCompileErrors(unsigned int shader, const std::string& type);
    void checkProgramLinkingErrors(unsigned int shader);
    void create_shader(const std::string& shaderFilepath);

    unsigned int programID;
    std::string vertexShaderFilepath;
    std::string fragmentShaderFilepath;
    std::string vertexShaderCode;
    std::string fragmentShaderCode;

};

#endif // SHADERPROGRAM_HPP
