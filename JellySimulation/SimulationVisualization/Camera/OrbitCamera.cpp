#include "pch.h"
#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(Vector3 target, float minDistance, float maxDistance, float distance)
	: m_angleX(0.0f), m_angleZ(0.0f), m_target(target.x, target.y, target.z), m_distance(distance)
{
	if (maxDistance < minDistance)
	{
		maxDistance = minDistance;
	}

	m_minDistance = minDistance;
	m_maxDistance = maxDistance;
}

OrbitCamera::OrbitCamera(float minDistance, float maxDistance, float distance)
	: OrbitCamera(Vector3{ 0.0f, 0.0f, 0.0f }, minDistance, maxDistance, distance)
{
}

Matrix OrbitCamera::GetViewMatrix()
{
	Vector3 position = GetEyePosition();

	Vector3 dir = GetFront();
	Vector3 up = Vector3::Transform(
		Vector3{ 0.0f, 0.0f, 1.0f },
		Matrix::CreateRotationX(m_angleX) * Matrix::CreateRotationZ(m_angleZ)
	);

	return Matrix::CreateLookAt(position, position + dir, up);
}

Vector3 OrbitCamera::GetEyePosition()
{
	auto dir = GetFront();
	return m_target - m_distance * dir;
}

Vector3 OrbitCamera::GetFront()
{
	Vector3 dir = Vector3::Transform(
		Vector3 { 0.0f, 1.0f, 0.0f }, 
		Matrix::CreateRotationX(m_angleX) * Matrix::CreateRotationZ(m_angleZ)
	);

	return dir;
}

Vector3 OrbitCamera::GetUp()
{
	Vector3 dir = Vector3::Transform(
		Vector3{ 0.0f, 0.0f, 1.0f },
		Matrix::CreateRotationX(m_angleX) * Matrix::CreateRotationZ(m_angleZ)
	);

	return dir;
}

Vector3 OrbitCamera::GetRight()
{
	Vector3 dir = Vector3::Transform(
		Vector3{ 1.0f, 0.0f, 0.0f },
		Matrix::CreateRotationZ(m_angleZ)
	);

	return dir;
}

void OrbitCamera::ProcessMouseMove(float dx, float dy)
{
	m_angleX = XMScalarModAngle(m_angleX - dy);
	m_angleZ = XMScalarModAngle(m_angleZ - dx);
}

void OrbitCamera::Zoom(float dd)
{
	m_distance += dd;
	ClampDistance();
}

void OrbitCamera::ClampDistance()
{
	if (m_distance < m_minDistance)
	{
		m_distance = m_minDistance;
	}

	if (m_distance > m_maxDistance)
	{
		m_distance = m_maxDistance;
	}
}
