#pragma once

#include <iostream>
#pragma once

#include "Transform.h"

class Entity
{
public:
	Transform transform;

	Entity(glm::vec3 position = glm::vec3(0.0f))
	{
		transform = Transform(position);
	}
};

