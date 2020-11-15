#pragma once

#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

class InputController
{
public:
	InputController(GLFWwindow* window, T* WindowPointer)
	{
		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

		glfwSetScrollCallback(window, OnMouseScroll);
	}

	static void OnMouseScroll(GLFWwindow* window, double _, double yOffset)
	{
		InputController* controller = reinterpret_cast<InputController*>(glfwGetWindowUserPointer(window));
	}	
};