#include "pch.h"
#include "Particle.h"

#include <chrono>

Particle::Particle(const Vector3& position)
	: m_position(position), m_startTime(0.0f), m_endTime(0.0f), m_percentage(1.0f)
{
}

Particle::Particle(const Vector3& position, float lifetime)
	: m_position(position), m_percentage(0.0f)
{
	using Clock = std::chrono::high_resolution_clock;
	auto now = std::chrono::duration<float>(Clock::now().time_since_epoch()).count();
	
	m_startTime = now;
	m_endTime = m_startTime + lifetime;
}

Particle::Particle(const Vector3& position, float startTime, float endTime)
	: m_position(position)
{
	using Clock = std::chrono::high_resolution_clock;
	auto now = std::chrono::duration<float>(Clock::now().time_since_epoch()).count();

	if (endTime <= startTime || endTime <= now)
	{
		m_percentage = 1.0f;
	}
	else
	{
		m_percentage = max(0.0f, (now - startTime) / (endTime - startTime));
	}

	m_startTime = startTime;
	m_endTime = endTime;
}
