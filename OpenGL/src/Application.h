#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Renderer.h"
#include "Entity.h"

#include "VertexArray.h"


class Application
{
public:
	Application();
	~Application();

	int Run();

private:
	GLFWwindow* window;

	std::vector<Entity> m_Entities;

	Renderer m_Renderer;

	VertexArray* m_VAO;	

	int Initialize();

	int Init_OpenGl();

	int CreateWindow();

	void LoadResources();

	void Input();

	void Render();
};

