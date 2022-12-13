#pragma once

#include "D11Device.h"

#include <Scene/Scene.h>

#include <memory>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class D11Renderer
{
public:
	D11Renderer();
	~D11Renderer();

	Matrix GetProjectionMatrix() const;

	HRESULT ResetRenderTarget(void* resource);
	HRESULT InitRenderTarget(void* resource);

	inline const D11Device& const Device() { return *m_device; }
	inline ID3D11DeviceContext* Context() { return m_device->Context().Get(); }

	inline UINT Width() { return m_width; }
	inline UINT Height() { return m_height; }

	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	void StartFrame();
	void Render(Scene& scene);
	void EndFrame();

private:
	void SetUpViewport();
	void SetUpStates();

	std::shared_ptr<D11Device> m_device;

	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	ComPtr<ID3D11BlendState> m_blendState;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	UINT m_width;
	UINT m_height;
};
