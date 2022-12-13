#pragma once

#include <DirectX/D11Renderer.h>

#include <ParticleSystem/ParticleSystem.h>

#include <Scene/Scene.h>
#include <Scene/SceneObject.h>

#include <memory>
#include <ctime> 

class App
{
public:
	App();

	void Update(float deltaTime);
	void Render(void* resource, bool isNewResource);

	void MoveCamera(float dx, float dy);
	void Zoom(float dd);

	void UpdatePhysics(float dt);

	// -- ACTIONS FROM UI
	void DisplayCubeChanged(bool display);
	void DisplayDiagonalChanged(bool display);
	void DisplayTraceChanged(bool display, uint32_t traceLength);
	void ToggleGravity(bool gravityOn);

	void RestartSimulation(float edge, float density, float deviation, float angularVelocity, float integrationStep, bool gravityOn);
	void PauseSimulation();
	void ResumeSimulation();

protected:

	float m_time_left_to_render;
	float m_integrationStep;
	bool m_simulationStarted = false;
	bool m_simulationPaused = false;

	float m_lastFrameTime;

	Scene m_scene;
	ParticleSystem m_particleSystem;
	std::shared_ptr<D11Renderer> m_renderer;

	SceneObject m_cube;
	SceneObject m_traceParticles;
	SceneObject m_grid;
	SceneObject m_diagonal;
};
