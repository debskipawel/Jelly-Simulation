#include "pch.h"
#include "ParticleEmitterComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent()
	: m_spawnerFn([]() { return Particle{}; }), m_updateFn([](Particle&, float) {}),
	m_particles(), m_maxParticles(0),
	m_periodPerParticle(std::numeric_limits<float>::infinity()), m_timeSinceLastEmit(0.0f)
{}

void ParticleEmitterComponent::Update(float dt)
{
	m_timeSinceLastEmit += dt;

	for (auto it = m_particles.begin(); it != m_particles.end(); it++)
	{
		auto& p = *it;

		if (p.m_startTime == p.m_endTime)
		{
			p.m_percentage = 1.0f;
		}
		else
		{
			p.m_percentage = p.m_percentage + dt / (p.m_endTime - p.m_startTime);
		}

		m_updateFn(p, dt);
	}

	std::remove_if(m_particles.begin(), m_particles.end(), [](const Particle& p) { return p.m_percentage >= 1.0f; });

	while (m_timeSinceLastEmit > m_periodPerParticle)
	{
		m_particles.emplace_back(m_spawnerFn());

		m_timeSinceLastEmit -= m_periodPerParticle;
	}

	auto particlesCount = m_particles.size();

	if (particlesCount > m_maxParticles)
	{
		auto particlesToDelete = particlesCount - m_maxParticles;
		m_particles.erase(m_particles.begin(), m_particles.begin() + particlesToDelete);
	}
}

void ParticleEmitterComponent::SetFrequency(float frequency)
{
	if (frequency <= 0.0f)
	{
		m_periodPerParticle = std::numeric_limits<float>::infinity();
	}

	m_periodPerParticle = 1.0f / frequency;
}
