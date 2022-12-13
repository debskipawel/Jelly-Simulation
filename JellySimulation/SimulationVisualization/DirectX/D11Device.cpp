#include "pch.h"

#include "D11Device.h"
#include "D11Structures.h"

D11Device::D11Device()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// DX10 or 11 devices are suitable
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		hr = D3D11CreateDevice(NULL, driverTypes[driverTypeIndex], NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, m_device.GetAddressOf(), &m_featureLevel, m_context.GetAddressOf());

		if (SUCCEEDED(hr))
		{
			m_driverType = driverTypes[driverTypeIndex];
			break;
		}
	}
}

ComPtr<ID3D11RenderTargetView> D11Device::CreateRenderTargetView(const ComPtr<ID3D11Texture2D>& texture)
{
	ComPtr<ID3D11RenderTargetView> result;
	auto hr = m_device->CreateRenderTargetView(texture.Get(), nullptr, result.GetAddressOf());

	if (FAILED(hr))
	{
		printf("Creating rtv failed\n");
	}

	return result;
}

ComPtr<ID3D11DepthStencilView> D11Device::CreateDepthStencilBuffer(UINT width, UINT height)
{
	auto desc = D11Texture2DDesc::DepthStencilDescription(width, height);
	auto tex = CreateTexture(desc);

	ComPtr<ID3D11DepthStencilView> result;
	auto hr = m_device->CreateDepthStencilView(tex.Get(), nullptr, result.GetAddressOf());
	tex.Reset();

	if (FAILED(hr))
	{
		printf("Error while creating depth buffer\n");
	}

	return result;
}

ComPtr<ID3D11Texture2D> D11Device::CreateTexture(const D3D11_TEXTURE2D_DESC& desc) const
{
	ComPtr<ID3D11Texture2D> result;
	auto hr = m_device->CreateTexture2D(&desc, nullptr, result.GetAddressOf());

	if (FAILED(hr))
	{
		printf("Error while creating a texture\n");
	}

	return result;
}
