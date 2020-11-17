#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	glm::vec3 position;

	Transform(glm::vec3 _position = glm::vec3(0.0f))
	{
		position = _position;
	}

	glm::mat4 GetModelMatrix()
	{
		return glm::translate(glm::mat4(1.0f), position);
	}
};