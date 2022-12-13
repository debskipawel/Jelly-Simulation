#include "pch.h"
#include "D11Shader.h"

#include <algorithm>

std::vector<ID3D11Buffer*> D11Shader::RawConstantBuffers()
{
    std::vector<ID3D11Buffer*> result(m_constantBuffers.size());
    
    std::transform(
        m_constantBuffers.begin(), m_constantBuffers.end(),
        result.begin(),
        [](std::shared_ptr<D11ConstantBuffer> b) { return b->Buffer(); }
    );

    return result;
}
