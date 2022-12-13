#include "pch.h"
#include "D11VertexBuffer.h"

#include <numeric>

D11VertexBuffer::D11VertexBuffer(const D11Device& device, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const void* data)
	: D11Buffer(device, data, GetBufferDesc(size)), m_offset(0)
{
	auto strideBit = std::accumulate(layout.begin(), layout.end(), (size_t)0, [](size_t a, D3D11_INPUT_ELEMENT_DESC desc) { return a + BitsPerPixel(desc.Format); });
	m_stride = strideBit / 8;
}

D11BufferDesc D11VertexBuffer::GetBufferDesc(size_t byteWidth) const
{
	auto res = D11BufferDesc{ D3D11_BIND_VERTEX_BUFFER, byteWidth };

	res.Usage = D3D11_USAGE_DYNAMIC;
	res.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return res;
}
