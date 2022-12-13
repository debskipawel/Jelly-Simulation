#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct PhysicsComponent
{
	PhysicsComponent(float angularVelocity, float density, float sideLength, Vector3 fieldForce)
		:  Density(density), SideLength(sideLength), FullSimulationTime(0.0f), FieldForce(fieldForce)
	{
		Mass = sideLength * sideLength * sideLength * density;
	
		InertiaTensor = Mass * sideLength * sideLength / 12 * Matrix(
			8.f, -3.f, -3.f, 0.f,
			-3.f, 8.f, -3.f, 0.f,
			-3.f, -3.f, 8.f, 0.f,
			0.f, 0.f, 0.f, 1.f);

		InverseInertiaTensor = 12 / (Mass * sideLength * sideLength) * Matrix(
			5.f/22.f, 3.f/22.f, 3.f/22.f, 0.f,
			3.f/22.f, 5.f / 22.f, 3.f/22.f, 0.f,
			3.f/22.f, 3.f/22.f, 5.f / 22.f, 0.f,
			0.f, 0.f, 0.f, 1.f);

		auto diagonalVecotr = Vector3(1, 1, 1);
		diagonalVecotr.Normalize();

		AngularVelocity = angularVelocity * diagonalVecotr;
	}

	Vector3 AngularVelocity;

	Matrix InertiaTensor;
	Matrix InverseInertiaTensor;

	Vector3 FieldForce;

	float FullSimulationTime;
	float Mass;
	float Density;
	float SideLength;
};
