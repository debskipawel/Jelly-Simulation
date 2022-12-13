#pragma once

#include "D11Buffer.h"

class D11IndexBuffer : public D11Buffer
{
public:
	explicit D11IndexBuffer(const D11Device& device, DXGI_FORMAT format, size_t size, const void* data = nullptr, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	inline UINT Count() const { return m_indicesCount; }
	inline DXGI_FORMAT Format() const { return m_format; }
	inline D3D_PRIMITIVE_TOPOLOGY Topology() { return m_topology; }

	virtual void Update(const void* data, size_t size) override;

protected:
	virtual D11BufferDesc GetBufferDesc(size_t byteWidth) const override;

	D3D_PRIMITIVE_TOPOLOGY m_topology;
	DXGI_FORMAT m_format;
	UINT m_indicesCount;
};
