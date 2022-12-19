#pragma once

#include <DirectX/D11Device.h>

using namespace Microsoft::WRL;

struct DepthStateComponent
{
public:
	DepthStateComponent(const D11Device& device)
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		device->CreateDepthStencilState(&dsDesc, DepthState.GetAddressOf());
	}

	ComPtr<ID3D11DepthStencilState> DepthState;
};
