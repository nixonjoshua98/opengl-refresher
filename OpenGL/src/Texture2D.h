#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture2D
{
public:
    unsigned int m_RendererID;

    unsigned int m_Width, m_Height; 

    Texture2D();

    void Generate(unsigned int width, unsigned int height, unsigned char* data);

    void Bind() const;
};