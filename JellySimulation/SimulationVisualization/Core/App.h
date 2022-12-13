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

	void UpdatePhysics();

protected:

	void InitializeControlPoints();

	float m_simulationTimeStep;

	float m_lastFrameTime;
	float m_fullSimulationTime;
	float m_residualSimulationTime;

	Scene m_scene;
	std::shared_ptr<D11Renderer> m_renderer;

	SceneObject m_grid;
	std::vector<SceneObject> m_controlPoints;
};
