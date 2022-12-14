#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct PhysicsComponent
{
	PhysicsComponent(float mass)
		:  Mass(mass)
	{
	}

	Vector3 Velocity;

	float Mass;
	Vector3 Forces;
};
