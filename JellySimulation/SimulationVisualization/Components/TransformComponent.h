#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct TransformComponent
{
	TransformComponent(Vector3 position, Quaternion rotation, Vector3 scaling)
		: Position(position), Rotation(rotation), Scaling(scaling)
	{}

	TransformComponent()
		: TransformComponent(Vector3::Zero, Quaternion::Identity, Vector3::One)
	{}

	TransformComponent(const TransformComponent& other) = default;

	Vector3 Position;
	Vector3 Scaling;
	Quaternion Rotation;

	operator Matrix() const
	{
		return Matrix::CreateScale(Scaling) * Matrix::CreateFromQuaternion(Rotation) * Matrix::CreateTranslation(Position);
	}
};
