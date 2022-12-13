#pragma once

#include <vector>

#include "Physics/DynamicSpring.h"

struct SpringsComponent
{
	SpringsComponent() = default;

	std::vector<DynamicSpring> springs;
};
