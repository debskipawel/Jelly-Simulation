#pragma once

#include <string>
#include <d3d11.h>

#include <Scene/SceneObject.h>

class ParticleSystem
{
public:
	ParticleSystem(Scene& scene);

	SceneObject AddInstance(const D11Device& device, std::wstring pixelShaderName, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	void Update(float dt);

protected:
	Scene* m_scene;
};
