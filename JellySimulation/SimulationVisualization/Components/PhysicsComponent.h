#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct PhysicsComponent
{
	PhysicsComponent(float mass)
		:  Mass(mass)
	{
	}

	float Mass;
};
