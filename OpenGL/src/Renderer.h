#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
public:
	void Clear() const;

	void Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const;
};