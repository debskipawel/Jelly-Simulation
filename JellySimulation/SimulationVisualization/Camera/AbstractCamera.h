#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class AbstractCamera
{
public:
	virtual Matrix GetViewMatrix() = 0;

	virtual Vector3 GetEyePosition() = 0;
	virtual Vector3 GetEyeDirection() = 0;

	virtual void ProcessMouseMove(float dx, float dy) = 0;
	virtual void Zoom(float dd) = 0;
};
