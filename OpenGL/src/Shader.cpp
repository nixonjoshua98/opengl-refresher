
#include "Shader.h"

#include <iostream>

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>


Shader::Shader() : m_RendererID(0)
{

}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Use() const
{
    glUseProgram(m_RendererID);
}

void Shader::Compile(const std::string& vertex, const std::string& fragment)
{
    m_RendererID = CreateShader(vertex, fragment);
}


void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}


void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int shader = glCreateShader(type);

    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    int result;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        int length;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        // Allocate the array on stack, instead of on heap
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(shader, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << "\n" << message << std::endl;

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}


unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_LocationCache.find(name) != m_LocationCache.end())
        return m_LocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());

    if (location == -1)
        std::cout << "Warning: Uniform '" << name << "' does not exist" << std::endl;

    m_LocationCache[name] = location;

    return location;
}