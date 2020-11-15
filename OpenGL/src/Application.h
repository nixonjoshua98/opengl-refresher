#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.h"
#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexArray.h"

struct GameObject;


class Application
{
public:
	Application();
	~Application();

	int Run();

private:
	GLFWwindow* window;

	Renderer m_Renderer;

	std::vector<GameObject> m_Objects;
	std::vector<Shader> m_Shaders;

	IndexBuffer* m_IBO;
	VertexArray* m_VAO;	

	void Input();

	void OnRender();

	int Initialize();

	int Init_OpenGl();

	int CreateWindow();
};

