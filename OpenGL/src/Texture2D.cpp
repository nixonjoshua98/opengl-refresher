#include <iostream>

#include "Texture2D.h"


Texture2D::Texture2D() : m_RendererID(0), m_Width(0), m_Height(0)
{

}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    m_Width = width;
    m_Height = height;

    glGenTextures(1, &m_RendererID);

    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}