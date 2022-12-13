#include "pch.h"
#include "App.h"

#include <Camera/OrbitCamera.h>

#include <Components/PhysicsComponent.h>
#include <Components/TransformComponent.h>
#include <Components/Particles/ParticleEmitterComponent.h>
#include <Components/Rendering/RenderingComponent.h>

#include <Scene/SceneObject.h>
#include <Scene/SceneIterator.h>
#include <Scene/EntityFactory.h>
#include "Algorithms/RungeKutty.h"
#include <Components/SpringsComponent.h>


App::App()
	: m_scene(),
	  m_lastFrameTime(0.0f), m_residualSimulationTime(0.0f),
	  m_renderer(std::make_shared<D11Renderer>()),
	  m_simulationTimeStep(0.01f)
{
	this->InitializeControlPoints();

	m_lastFrameTime = Clock::Now();

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
	for (auto it = m_scene.Begin<PhysicsComponent>(); it != m_scene.End<PhysicsComponent>(); ++it)
	{
		auto entity = it.Get();
		auto& component = entity.GetComponent<PhysicsComponent>();
	}
}

void App::InitializeControlPoints()
{
	constexpr float cubeSide = 1.0f;
	constexpr float distanceBetweenPoints = cubeSide / 3;
	const Vector3 initialPoint = -cubeSide / 2 * Vector3::One;

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
		transform.Position = initialPoint + Vector3{ x * distanceBetweenPoints, y * distanceBetweenPoints, z * distanceBetweenPoints };

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

			springsComponent.springs.push_back(DynamicSpring{ point2, positionDifference.Length() });
		}
	}
}
