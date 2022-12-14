#include "pch.h"
#include "App.h"

#include <Camera/OrbitCamera.h>

#include <Components/PhysicsComponent.h>
#include <Components/TransformComponent.h>
#include <Components/SpringsComponent.h>
#include <Components/Particles/ParticleEmitterComponent.h>
#include <Components/Rendering/RenderingComponent.h>

#include <Scene/SceneObject.h>
#include <Scene/SceneIterator.h>
#include <Scene/EntityFactory.h>

constexpr float CUBE_SIDE = 1.0f;
constexpr float DISTANCE_BETWEEN_POINTS = CUBE_SIDE / 3;

App::App()
	: m_scene(),
	  m_lastFrameTime(0.0f), m_residualSimulationTime(0.0f),
	  m_renderer(std::make_shared<D11Renderer>()),
	  m_simulationTimeStep(0.01f), m_controlPointMass(1.0f)
{
	m_lastFrameTime = Clock::Now();
	m_renderSteeringFrame = EntityFactory::CreateCube(m_renderer->Device(), m_scene);

	m_grid = EntityFactory::CreateWorldGrid(m_renderer->Device(), m_scene);
}

void App::Update(float deltaTime)
{
	m_residualSimulationTime += deltaTime;

	while (m_residualSimulationTime > m_simulationTimeStep)
	{
		m_fullSimulationTime += m_simulationTimeStep;
		m_residualSimulationTime -= m_simulationTimeStep;

		UpdatePhysics();
	}

	UpdateMesh();
}

void App::Render(void* resource, bool isNewResource)
{
	auto now = Clock::Now();
	auto deltaTime = now - m_lastFrameTime;
	m_lastFrameTime = now;

	Update(deltaTime);

	if (isNewResource)
	{
		m_renderer->ResetRenderTarget(resource);
	}

	m_renderer->StartFrame();
	m_renderer->Clear(0.1f, 0.1f, 0.1f);
	m_renderer->Render(m_scene);
	m_renderer->EndFrame();
}

void App::MoveSteeringCube(float dx, float dy)
{
	auto camera = m_scene.Camera();
	auto move = dx * camera->GetRight() + dy * camera->GetUp();

	auto& transform = m_renderSteeringFrame.GetComponent<TransformComponent>();
	transform.Position += move;

	// TODO: dodac kolizje
}

void App::MoveCamera(float dx, float dy)
{
	m_scene.Camera()->ProcessMouseMove(dx, dy);
}

void App::Zoom(float dd)
{
	m_scene.Camera()->Zoom(dd);
}

void App::UpdatePhysics()
{
	for (auto it = m_scene.Begin<SpringsComponent>(); it != m_scene.End<SpringsComponent>(); ++it)
	{
		auto entity = it.Get();
		
		auto& springs = entity.GetComponent<SpringsComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& physics = entity.GetComponent<PhysicsComponent>();

		auto position = transform.Position;

		for (auto& spring : springs.springs)
		{
			auto attachment = spring.attached;
			auto attachedPosition = attachment.GetComponent<TransformComponent>().Position;
			auto attachedVelocity = attachment.GetComponent<PhysicsComponent>().Velocity;

			auto positionDifference = attachedPosition - position;
			auto relativeVelocity = attachedVelocity - physics.Velocity;

			auto forceDirection = positionDifference;
			forceDirection.Normalize();

			auto deviation = positionDifference.Length() - spring.initialLength;
			auto deviationDeriv = forceDirection.Dot(relativeVelocity);
			
			auto forceValue = -(m_stickiness * deviationDeriv + spring.elasticity * deviation);

			physics.Forces += forceDirection * forceValue;
		}
	}

	for (auto it = m_scene.Begin<PhysicsComponent>(); it != m_scene.End<PhysicsComponent>(); ++it)
	{
		auto entity = it.Get();
		auto& component = entity.GetComponent<PhysicsComponent>();
	}
}

void App::UpdateVisualizationParameters(bool drawControlPoints, bool drawSteeringCube, bool drawShadedCube)
{
	auto& renderCP = m_renderControlPoints.GetComponent<RenderingComponent>();
	renderCP.ShouldRender = drawControlPoints;

	auto& renderSF = m_renderSteeringFrame.GetComponent<RenderingComponent>();
	renderSF.ShouldRender = drawSteeringCube;

	auto& renderSC = m_renderShadedCube.GetComponent<RenderingComponent>();
	renderSC.ShouldRender = drawShadedCube;
}

void App::RestartSimulation(float pointMass, float stickiness, float massesElasticity, float steeringSpringsElasticity, float maxImbalance)
{
	m_controlPointMass = pointMass;
	m_stickiness = stickiness;
	m_elasticityBetweenMasses = massesElasticity;
	m_elasticityOnSteeringSprings = steeringSpringsElasticity;
	m_maxInitialImbalance = maxImbalance;

	m_fullSimulationTime = 0.0f;
	m_residualSimulationTime = 0.0f;

	this->InitializeControlPoints();
}

void App::InitializeControlPoints()
{
	const Vector3 cubeCenterPosition = Vector3::Zero;
	const Vector3 initialPoint = -CUBE_SIDE / 2 * Vector3::One + cubeCenterPosition;

	for (auto& point : m_controlPoints)
	{
		point.Destroy();
	}

	m_controlPoints.clear();

	for (int i = 0; i < 64; i++)
	{
		int x = i % 4;
		int y = (i / 4) % 4;
		int z = i / 16;

		SceneObject point(m_scene);
		
		auto& transform = point.AddComponent<TransformComponent>();
		transform.Position = initialPoint + Vector3{ x * DISTANCE_BETWEEN_POINTS, y * DISTANCE_BETWEEN_POINTS, z * DISTANCE_BETWEEN_POINTS };

		point.AddComponent<PhysicsComponent>(m_controlPointMass);

		m_controlPoints.push_back(point);
	}

	for (int i = 0; i < 64; i++)
	{
		auto& point1 = m_controlPoints[i];
		auto& transform1 = point1.GetComponent<TransformComponent>();
		auto& springsComponent = point1.AddComponent<SpringsComponent>();

		int x1 = i % 4;
		int y1 = (i / 4) % 4;
		int z1 = i / 16;

		for (int j = 0; j < 64; j++)
		{
			int x2 = j % 4;
			int y2 = (j / 4) % 4;
			int z2 = j / 16;

			if (abs(x2 - x1) != 1 && abs(y2 - y1) != 1 && abs(z2 - z1) != 1)
			{
				continue;
			}

			auto& point2 = m_controlPoints[j];
			auto& transform2 = point2.GetComponent<TransformComponent>();

			auto positionDifference = transform2.Position - transform1.Position;

			springsComponent.springs.push_back(DynamicSpring{ point2, positionDifference.Length(), m_elasticityBetweenMasses });
		}
	}

	auto imbalance = m_maxInitialImbalance;

	std::for_each(std::execution::par, m_controlPoints.begin(), m_controlPoints.end(),
		[imbalance](SceneObject object)
		{
			Random r;
			float dx = r.Next(-imbalance, imbalance);
			float dy = r.Next(-imbalance, imbalance);
			float dz = r.Next(-imbalance, imbalance);

			auto& transform = object.GetComponent<TransformComponent>();
			transform.Position += Vector3{ dx, dy, dz };
		}
	);
}

void App::UpdateMesh()
{
}
