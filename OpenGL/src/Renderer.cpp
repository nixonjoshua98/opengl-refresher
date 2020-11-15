#include "Renderer.h"

#include <GL/glew.h>

void Renderer::Clear() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const
{
    shader.Bind();

    VAO.Bind();

    IBO.Bind();
  
    glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, 0);
}