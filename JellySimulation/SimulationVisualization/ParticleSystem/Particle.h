#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct Particle
{
	Particle() = default;
	Particle(const Vector3& position);
	Particle(const Vector3& position, float lifetime);
	Particle(const Vector3& position, float startTime, float endTime);


	Vector3 m_position;
	float m_startTime, m_endTime;
	float m_percentage;
};
