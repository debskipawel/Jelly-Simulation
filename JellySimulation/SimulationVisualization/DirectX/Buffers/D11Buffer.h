#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <DirectX/D11Structures.h>
#include <DirectX/D11Device.h>

using namespace Microsoft::WRL;

class D11Buffer
{
public:
	D11Buffer(const D11Device& device, const void* data, D11BufferDesc desc);
	virtual ~D11Buffer() = default;

	inline ID3D11Buffer* Buffer() const { return m_buffer.Get(); }

	virtual void Update(const void* data, size_t size);

protected:
	virtual D11BufferDesc GetBufferDesc(size_t byteWidth) const = 0;

	virtual void CreateBuffer(const void* data, const D11BufferDesc& desc);

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;

	D11BufferDesc m_desc;
	ComPtr<ID3D11Buffer> m_buffer;
};

size_t BitsPerPixel(DXGI_FORMAT fmt);
