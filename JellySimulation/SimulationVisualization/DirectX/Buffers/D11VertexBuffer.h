#pragma once

#include "D11Buffer.h"

#include <vector>

class D11VertexBuffer : public D11Buffer
{
public:
	explicit D11VertexBuffer(const D11Device& device, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const void* data = nullptr);

	inline UINT Stride() const { return m_stride; }
	inline UINT Offset() const { return m_offset; }

protected:
	virtual D11BufferDesc GetBufferDesc(size_t byteWidth) const override;

	size_t m_stride;
	size_t m_offset;
};
