#pragma once

#include "D11Buffer.h"

class D11ConstantBuffer : public D11Buffer
{
public:
	explicit D11ConstantBuffer(const D11Device& device, size_t size, const void* data = nullptr)
		: D11Buffer(device, data, GetBufferDesc(size)) {}

protected:
	virtual D11BufferDesc GetBufferDesc(size_t byteWidth) const override;
};
