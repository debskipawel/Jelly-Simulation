#include "pch.h"
#include "D11IndexBuffer.h"

D11IndexBuffer::D11IndexBuffer(const D11Device& device, DXGI_FORMAT format, size_t size, const void* data, D3D_PRIMITIVE_TOPOLOGY topology)
	: D11Buffer(device, data, GetBufferDesc(size)), m_format(format), m_topology(topology)
{
	auto bpp = BitsPerPixel(format);
	m_indicesCount = size / (bpp / 8);
}

void D11IndexBuffer::Update(const void* data, size_t size)
{
	D11Buffer::Update(data, size);

	m_indicesCount = size / (BitsPerPixel(m_format) / 8);
}

D11BufferDesc D11IndexBuffer::GetBufferDesc(size_t byteWidth) const
{
	auto res = D11BufferDesc{ D3D11_BIND_INDEX_BUFFER, byteWidth };

	res.Usage = D3D11_USAGE_DYNAMIC;
	res.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return res;
}
