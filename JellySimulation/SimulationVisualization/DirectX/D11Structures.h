#pragma once

#include <d3d11.h>

struct D11SwapchainDesc : public DXGI_SWAP_CHAIN_DESC
{
	D11SwapchainDesc(HWND wndHwnd, UINT width, UINT height);
};

struct D11Texture2DDesc : D3D11_TEXTURE2D_DESC
{
	D11Texture2DDesc(UINT width, UINT height);

	static D11Texture2DDesc DepthStencilDescription(UINT width, UINT height);
};

struct D11Viewport : D3D11_VIEWPORT
{
	explicit D11Viewport(UINT width, UINT height);
};

struct D11BufferDesc : D3D11_BUFFER_DESC
{
	D11BufferDesc(UINT bindFlags, size_t byteWidth);

	inline static D11BufferDesc VertexBufferDescription(size_t byteWidth)
	{
		return { D3D11_BIND_VERTEX_BUFFER, byteWidth };
	}

	inline static D11BufferDesc IndexBufferDescription(size_t byteWidth)
	{
		return { D3D11_BIND_INDEX_BUFFER, byteWidth };
	}

	inline static D11BufferDesc ConstantBufferDescription(size_t byteWidth)
	{
		D11BufferDesc desc{ D3D11_BIND_CONSTANT_BUFFER, byteWidth };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		return desc;
	}
};

struct SamplerDescription : D3D11_SAMPLER_DESC
{
	SamplerDescription();
};
