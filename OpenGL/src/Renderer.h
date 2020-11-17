#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "Shader.h"


class Renderer
{
public:
    static void Clear()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    static void DrawCube(VertexArray* VAO, Shader* shader)
    {
        shader->Use();

        VAO->Bind();

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
};