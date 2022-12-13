#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class AbstractCamera
{
public:
	virtual Matrix GetViewMatrix() = 0;

	virtual Vector3 GetEyePosition() = 0;
	virtual Vector3 GetFront() = 0;
	virtual Vector3 GetUp() = 0;
	virtual Vector3 GetRight() = 0;

	virtual void ProcessMouseMove(float dx, float dy) = 0;
	virtual void Zoom(float dd) = 0;
};
