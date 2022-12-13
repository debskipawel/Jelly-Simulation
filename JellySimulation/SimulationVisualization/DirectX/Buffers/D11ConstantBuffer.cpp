#include "pch.h"
#include "D11ConstantBuffer.h"

D11BufferDesc D11ConstantBuffer::GetBufferDesc(size_t byteWidth) const
{
    return D11BufferDesc::ConstantBufferDescription(byteWidth);
}
