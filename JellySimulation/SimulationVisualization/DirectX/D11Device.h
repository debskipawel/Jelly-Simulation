#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class D11Device
{
public:
	explicit D11Device();

	inline ID3D11Device* Raw() const { return m_device.Get(); }
	inline ID3D11Device* operator->() const { return Raw(); }
	inline const ComPtr<ID3D11DeviceContext>& Context() const { return m_context; }

	ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(const ComPtr<ID3D11Texture2D>& texture);
	ComPtr<ID3D11DepthStencilView> CreateDepthStencilBuffer(UINT width, UINT height);
	ComPtr<ID3D11Texture2D> CreateTexture(const D3D11_TEXTURE2D_DESC& desc) const;
	ComPtr<ID3D11ShaderResourceView> CreateShaderResourceViewFromFile(const std::wstring& texPath) const;

private:

	D3D_FEATURE_LEVEL m_featureLevel;
	D3D_DRIVER_TYPE m_driverType;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
};
