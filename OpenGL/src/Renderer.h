#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "ResourceCache.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Entity.h"


class Renderer
{
public:
    static void Clear()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    template<class T>
    static void DrawEntities(std::vector<T*> entities, const VertexArray* VAO, const glm::mat4& projection, const glm::mat4 view)
    {
        Shader* shader = ResourceCache::GetShader("s_Basic");
        Texture2D* texture = ResourceCache::GetTexture("t_Basic");

        VAO->Bind();

        shader->Use();

        shader->SetUniform1i("u_Texture", 0);

        shader->SetUniformMat4f("u_View", view);

        shader->SetUniformMat4f("u_Projection", projection);

        for (auto entity : entities)
        {
            shader->SetUniformMat4f("u_Model", entity->transform.GetModelMatrix());

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
};