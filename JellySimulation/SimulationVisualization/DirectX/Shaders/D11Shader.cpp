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

std::vector<ID3D11ShaderResourceView*> D11Shader::RawTextures()
{
    std::vector<ID3D11ShaderResourceView*> result(m_textures.size());

    std::transform(
        m_textures.begin(), m_textures.end(),
        result.begin(),
        [](ComPtr<ID3D11ShaderResourceView> b) { return b.Get(); }
    );

    return result;
}
