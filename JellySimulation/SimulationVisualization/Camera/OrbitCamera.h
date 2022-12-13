#pragma once

#include "AbstractCamera.h"

class OrbitCamera : public AbstractCamera
{
public:
	explicit OrbitCamera(Vector3 target = Vector3(0, 0, 0),
		float minDistance = 0.0f, float maxDistance = FLT_MAX, float distance = 0.0f);
	explicit OrbitCamera(float minDistance, float maxDistance = FLT_MAX, float distance = 0.0f);

	virtual Matrix GetViewMatrix() override;
	
	virtual Vector3 GetEyePosition() override;
	
	virtual Vector3 GetFront() override;
	virtual Vector3 GetUp() override;
	virtual Vector3 GetRight() override;

	virtual void ProcessMouseMove(float dx, float dy) override;
	virtual void Zoom(float dd) override;

protected:
	void ClampDistance();

	Vector3 m_target;
	
	float m_angleX, m_angleZ;
	float m_distance, m_minDistance, m_maxDistance;
};
