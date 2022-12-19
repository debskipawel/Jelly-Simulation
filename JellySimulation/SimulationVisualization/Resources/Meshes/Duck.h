#pragma once
#include <vector>
#include <SimpleMath.h>

#include <d3d11.h>


struct VertexPositionNormalTex
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Vector2 tex;

	static const D3D11_INPUT_ELEMENT_DESC Layout[3];
};

static const std::vector<D3D11_INPUT_ELEMENT_DESC> g_duckLayout = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexPositionNormalTex, position), 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPositionNormalTex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPositionNormalTex, tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
