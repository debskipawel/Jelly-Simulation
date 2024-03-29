#include "pch.h"
#include "D11Structures.h"

D11SwapchainDesc::D11SwapchainDesc(HWND wndHwnd, UINT width, UINT height)
{
	ZeroMemory(this, sizeof(D11SwapchainDesc));
	BufferDesc.Width = 0;
	BufferDesc.Height = 0;
	BufferDesc.RefreshRate.Numerator = 120;
	BufferDesc.RefreshRate.Denominator = 1;
	BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //0
	BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	SampleDesc.Quality = 0;
	SampleDesc.Count = 1;
	BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	BufferCount = 1;
	OutputWindow = wndHwnd;
	Windowed = true;
	//SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //0
	//Flags = 0;
}

D11BufferDesc::D11BufferDesc(UINT bindFlags, size_t byteWidth)
{
	ZeroMemory(this, sizeof(D11BufferDesc));
	BindFlags = bindFlags;
	ByteWidth = static_cast<UINT>(byteWidth);
	Usage = D3D11_USAGE_DEFAULT;
	//CPUAccessFlags = 0;
	//MiscFlags = 0;
	//StructureByteStride = 0;
}

D11Texture2DDesc::D11Texture2DDesc(UINT width, UINT height)
{
	ZeroMemory(this, sizeof(D11Texture2DDesc));
	Width = width;
	Height = height;
	//MipLevels = 0;
	ArraySize = 1;
	Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SampleDesc.Count = 1;
	SampleDesc.Quality = 0;
	Usage = D3D11_USAGE_DEFAULT;
	BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//CPUAccessFlags = 0;
	//MiscFlags = 0;
}

D11Texture2DDesc D11Texture2DDesc::DepthStencilDescription(UINT width, UINT height)
{
	auto res = D11Texture2DDesc(width, height);

	res.MipLevels = 1;
	res.Format = DXGI_FORMAT_D32_FLOAT;
	res.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	return res;
}

D11Viewport::D11Viewport(UINT width, UINT height)
{
	TopLeftX = 0.0f;
	TopLeftY = 0.0f;
	Width = static_cast<FLOAT>(width);
	Height = static_cast<FLOAT>(height);
	MinDepth = 0.0f;
	MaxDepth = 1.0f;
}

SamplerDescription::SamplerDescription()
{
	Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	MinLOD = -D3D11_FLOAT32_MAX;
	MaxLOD = D3D11_FLOAT32_MAX;
	MipLODBias = 0.0f;
	MaxAnisotropy = 1;
	ComparisonFunc = D3D11_COMPARISON_NEVER;
	BorderColor[0] = 1.0f;
	BorderColor[1] = 1.0f;
	BorderColor[2] = 1.0f;
	BorderColor[3] = 1.0f;
}
