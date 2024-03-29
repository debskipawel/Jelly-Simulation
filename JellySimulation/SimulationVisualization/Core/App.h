#pragma once

#include <DirectX/D11Renderer.h>

#include <ParticleSystem/ParticleSystem.h>

#include <Scene/Scene.h>
#include <Scene/SceneObject.h>

#include <Resources/Structs/SpringDependentEntity.h>

#include <memory>
#include <ctime> 

class App
{
public:
	App();

	void Update(float deltaTime);
	void Render(void* resource, bool isNewResource);

	void RotateSteeringCube(float dx, float dy);
	void MoveSteeringCube(float dx, float dy);
	void MoveCamera(float dx, float dy, bool rotate);
	void Zoom(float dd);

	void ToggleGravity(bool gravityOn);
	void UpdatePhysics();
	void UpdateVisualizationParameters(bool drawControlPoints, bool drawSteeringCube, bool drawShadedCube, bool drawBoundingCuboid, bool drawDuck);

	void RestartSimulation(float pointMass, float stickiness, float massesElasticity, float steeringSpringsElasticity, float steeringElasticyOnCollisions, float maxImbalance);

protected:

	void InitializeControlPoints();
	void InitializeControlFrame();
	
	void InitializeMesh();
	void UpdateMesh();

	bool ApplyCollisions();

	float m_elasticityBetweenMasses;
	float m_elasticityOnSteeringSprings;
	float m_elasticyOnCollisions;
	float m_stickiness;
	float m_controlPointMass;
	float m_maxInitialImbalance;

	float m_simulationTimeStep;

	float m_lastFrameTime;
	float m_fullSimulationTime;
	float m_residualSimulationTime;

	Vector3 m_outerForces = Vector3::Zero;

	Scene m_scene;
	std::shared_ptr<D11Renderer> m_renderer;

	SceneObject m_grid;
	SceneObject m_mesh;

	std::vector<SpringDependentEntity> m_controlPoints;
	
	std::vector<SceneObject> m_controlFrame;
	std::vector<Vector3> m_controlFramePositions;
	float m_xAngle = 0.0f, m_zAngle = 0.0f;
	Vector3 m_controlFrameCenter = Vector3::Zero;

	SceneObject m_renderControlPoints;
	SceneObject m_renderDuck;
	SceneObject m_renderSteeringFrame;
	SceneObject m_renderShadedCube;

	SceneObject m_renderBoundingCuboid;

	const float boundingCubeSideLength = 10.0f;

};
