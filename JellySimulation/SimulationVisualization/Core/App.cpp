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


App::App()
	: m_scene(), m_particleSystem(m_scene),
	  m_lastFrameTime(0.0f), m_time_left_to_render(0.0f),
	  m_renderer(std::make_shared<D11Renderer>()),
	  m_integrationStep()
{
	m_lastFrameTime = Clock::Now();

	m_grid = EntityFactory::CreateWorldGrid(m_renderer->Device(), m_scene);
	m_cube = EntityFactory::CreateCube(m_renderer->Device(), m_scene, 1.0f);
	
	auto& transform = m_cube.GetComponent<TransformComponent>();
	transform.Position = Vector3(0, 0, 0.0f);
	
	m_diagonal = EntityFactory::CreateDiagonal(m_renderer->Device(), m_scene, 1.0f);

	m_traceParticles = m_particleSystem.AddInstance(m_renderer->Device(), L"../shaders_bin/solid_white_ps.hlsl", D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	auto& particleComponent = m_traceParticles.GetComponent<ParticleEmitterComponent>();
	particleComponent.SetFrequency(0.0f);
}

void App::Update(float deltaTime)
{
	m_particleSystem.Update(deltaTime);
	UpdatePhysics(deltaTime);
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

void App::UpdatePhysics(float dt)
{
	for (auto it = m_scene.Begin<PhysicsComponent>(); it != m_scene.End<PhysicsComponent>(); ++it)
	{
		auto entity = it.Get();
		auto& component = entity.GetComponent<PhysicsComponent>();

		if (!m_simulationPaused)
		{
			component.FullSimulationTime += dt;
		}
	}

	if (m_simulationPaused)
	{
		return;
	}

	if (m_cube.HasComponent<PhysicsComponent>())
	{
		m_time_left_to_render += dt;
		
		while (m_time_left_to_render > m_integrationStep)
		{
			m_time_left_to_render -= m_integrationStep;

			auto& component = m_cube.GetComponent<PhysicsComponent>();
			auto& transform = m_cube.GetComponent<TransformComponent>();

			auto matrixRotation = Matrix::CreateFromQuaternion(transform.Rotation);
			auto negativeQuaternion = transform.Rotation;
			negativeQuaternion.w = -negativeQuaternion.w;
			auto matrixReversedRotation = Matrix::CreateFromQuaternion(negativeQuaternion);

			auto fromCubeToSceneMatrix = Matrix::CreateRotationZ(XM_PIDIV4) * Matrix::CreateRotationX(acosf(1.0f / sqrtf(3))) * matrixRotation;
			auto fromSceneToCubeMatrix = matrixReversedRotation * Matrix::CreateRotationX(-acosf(1.0f / sqrtf(3))) * Matrix::CreateRotationZ(-XM_PIDIV4);

			auto inertiaTensor = component.InertiaTensor;
			auto inverseInertiaTensor =  component.InverseInertiaTensor;

			auto sideLength = component.SideLength;
			auto acceleration = component.FieldForce;
			auto mass = component.Mass;
			auto force = acceleration * mass;
			auto Force = Vector3::Transform(force, fromSceneToCubeMatrix);
			auto CenterOfMass = Vector3(sideLength / 2, sideLength / 2, sideLength / 2);
			auto N = CenterOfMass.Cross(Force);

			std::function<Vector3(double, Vector3)> fty = [inertiaTensor, inverseInertiaTensor, N](double t, Vector3 W) {

				auto IW = Vector3(Vector4::Transform(Vector4(W), inertiaTensor));
				auto IWW = IW.Cross(W);
				auto NIWW = N + IWW;
				auto I1NIWW = Vector3(Vector4::Transform(Vector4(NIWW), inverseInertiaTensor));
				return I1NIWW;
			};

			auto W1 = component.AngularVelocity;
			auto W2 = RungeKutty(component.FullSimulationTime, W1, m_integrationStep, fty);
			component.AngularVelocity = W2;

			std::function<Quaternion(double, Quaternion)> fty2 = [W2](double t, Quaternion Q) {
				Quaternion Qt;
				auto W22 = (W2 / 2.f);
				Quaternion result = Q * Quaternion(W22.x, W22.y, W22.z, 0);
				return result;
			};

			auto q1 = transform.Rotation;

			auto q1xyz = Vector3(q1.x, q1.y, q1.z);
			auto Q1xyz = Vector3(Vector4::Transform(Vector4(q1xyz), fromSceneToCubeMatrix));

			auto Q1 = Quaternion(Q1xyz.x, Q1xyz.y, Q1xyz.z, q1.w);

			auto Q2 = RungeKutty(component.FullSimulationTime, Q1, m_integrationStep, fty2);
			Q2.Normalize();

			auto Q2xyz = Vector3(Q2.x, Q2.y, Q2.z);
			auto q2xyz = Vector3::Transform(Q2xyz, fromCubeToSceneMatrix);
			auto q2 = Quaternion(q2xyz.x, q2xyz.y, q2xyz.z, Q2.w);

			transform.Rotation = q2;

			auto& diagonalTransform = m_diagonal.GetComponent<TransformComponent>();
			diagonalTransform.Rotation = transform.Rotation;
		}
	}
}

void App::DisplayCubeChanged(bool display)
{
	auto& render = m_cube.GetComponent<RenderingComponent>();
	render.ShouldRender = display;
}

void App::DisplayDiagonalChanged(bool display)
{
	auto& render = m_diagonal.GetComponent<RenderingComponent>();
	render.ShouldRender = display;
}

void App::DisplayTraceChanged(bool display, uint32_t traceLength)
{
	auto& particles = m_traceParticles.GetComponent<ParticleEmitterComponent>();
	particles.SetMaxParticles(display ? traceLength : 0);
}

void App::ToggleGravity(bool gravityOn)
{
	for (auto it = m_scene.Begin<PhysicsComponent>(); it != m_scene.End<PhysicsComponent>(); ++it)
	{
		auto entity = it.Get();
		auto& component = entity.GetComponent<PhysicsComponent>();

		component.FieldForce = (gravityOn ? 1 : 0) * Vector3{ 0.0f, 0.0f, -9.81f };
	}
}

void App::RestartSimulation(float edge, float density, float deviation, float angularVelocity, float integrationStep, bool gravityOn)
{
	m_integrationStep = integrationStep;
	m_time_left_to_render = 0.0f;

	m_simulationPaused = false;
	m_simulationStarted = true;

	m_cube.Destroy();
	auto cube = EntityFactory::CreateCube(m_renderer->Device(), m_scene, edge);

	auto fieldForce = (gravityOn ? 1 : 0) * Vector3{ 0.0f, 0.0f, -9.81f };
	cube.AddComponent<PhysicsComponent>(angularVelocity, density, edge, fieldForce);
	
	auto& transform = cube.GetComponent<TransformComponent>();

	transform.Rotation = Quaternion::CreateFromAxisAngle({ 0,1,0 }, XMConvertToRadians(deviation));
	transform.Position = Vector3(0, 0, 0.0f);

	auto& particles = m_traceParticles.GetComponent<ParticleEmitterComponent>();
	particles.Clear();
	particles.SetParticleSpawnerFn(
		[cube, edge]()
		{
			auto& transform = cube.GetComponent<TransformComponent>();
			Vector3 pos = Vector3::Transform({ 0.0f, 0.0f, edge * sqrtf(3) }, transform);
			return Particle(pos, 100.0f);
		}
	);
	particles.SetFrequency(600.0f);
	
	auto& diagonalTransform = m_diagonal.GetComponent<TransformComponent>();
	diagonalTransform.Scaling.z = edge * sqrtf(3);
	diagonalTransform.Rotation = transform.Rotation;

	m_cube = cube;
}

void App::PauseSimulation()
{
	m_simulationPaused = true;
}

void App::ResumeSimulation()
{
	m_simulationPaused = false;
}
