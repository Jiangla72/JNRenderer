#pragma once
#include "Base/core.h"
#include <glm/glm.hpp>

struct JNAPI Light
{
	glm::vec4 position;
	glm::vec4 intensity;
	glm::vec4 color;
	Light() {};
};