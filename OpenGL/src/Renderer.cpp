#include "Renderer.h"

#include <GL/glew.h>

void Renderer::Clear() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::DrawCube(VertexArray* VAO, Shader* shader) const
{
    shader->Use();

    VAO->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
}