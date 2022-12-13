#pragma once

#include <vector>
#include <d3d11.h>

#include <Scene/SceneObject.h>
#include <ParticleSystem/Particle.h>

class ParticleEmitterComponent
{
public:
	ParticleEmitterComponent();

	void Update(float dt);

	inline void SetParticleSpawnerFn(std::function<Particle()> spawnerFn)
	{
		m_spawnerFn = spawnerFn;
	}

	inline void SetParticleUpdateFn(std::function<void(Particle&, float)> updateFn)
	{
		m_updateFn = updateFn;
	}

	inline void Clear()
	{
		m_particles.clear();
	}

	inline float Frequency() const { return 1.0f / m_periodPerParticle; }
	void SetFrequency(float frequency);

	inline uint32_t MaxParticles() const { return m_maxParticles; }
	inline void SetMaxParticles(uint32_t maxParticles) { m_maxParticles = maxParticles; }

	inline const std::vector<Particle>& Particles() const { return m_particles; }

protected:

	float m_timeSinceLastEmit;
	float m_periodPerParticle;

	std::function<Particle()> m_spawnerFn;
	std::function<void(Particle&, float)> m_updateFn;
	std::vector<Particle> m_particles;
	uint32_t m_maxParticles;
};
