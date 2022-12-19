#pragma once

#include <DirectX/D11Device.h>

using namespace Microsoft::WRL;

struct BlendStateComponent
{
public:
	BlendStateComponent(const D11Device& device)
	{
        D3D11_BLEND_DESC omDesc;
        ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));

        omDesc.RenderTarget[0].BlendEnable = true;
        omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
        omDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ZERO;
        omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
        omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
        omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        omDesc.AlphaToCoverageEnable = false;

		device->CreateBlendState(&omDesc, BlendState.GetAddressOf());
	}


	ComPtr<ID3D11BlendState> BlendState;
};
