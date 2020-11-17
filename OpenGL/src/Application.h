#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Camera.h"
#include "Block.h"

#include "VertexArray.h"


class Application
{
public:
	Application();
	~Application();

	int Run();

	Camera* camera;

private:
	GLFWwindow* window;

	std::vector<Block> m_Entities;

	VertexArray* m_VAO;	

	int Initialize();

	int Init_OpenGl();

	int CreateWindow();

	void Setup();

	void Input();

	void Render();

	static void MouseMovementCallback(GLFWwindow* window, double xPos, double yPos);
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
};

