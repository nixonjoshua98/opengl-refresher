#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "Shader.h"


class Renderer
{
public:
	void Clear() const;

	void DrawCube(VertexArray* VAO, Shader* shader) const;
};