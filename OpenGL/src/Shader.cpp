
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>


Shader::Shader()
{
}

Shader::Shader(const std::string& filepath) : m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);

    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
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

    const char* src = source.c_str();  // Raw pointer

    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    // Validate compilatation
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


ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::fstream stream(filepath);

    std::string line;

    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            // Set shader to vertex
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            // Set shader to fragment
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
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
