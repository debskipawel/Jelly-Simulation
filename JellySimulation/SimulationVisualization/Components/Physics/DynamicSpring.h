#pragma once

#include <Scene/SceneObject.h>

struct DynamicSpring
{
	SceneObject attached;
	float initialLength;
	float elasticity;
};
