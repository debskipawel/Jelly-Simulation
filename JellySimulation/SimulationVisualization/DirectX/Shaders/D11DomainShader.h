#pragma once

#include "D11Shader.h"

class D11DomainShader : public D11Shader
{
public:
	explicit D11DomainShader(const D11Device& device, void* code, size_t size)
	{
		if (FAILED(device->CreateDomainShader(code, size, nullptr, m_shader.GetAddressOf())))
		{
			// TODO: log errors
			//printf("Error while creating hull shader\n");
		}
	}

	inline ID3D11DomainShader* Shader() { return m_shader.Get(); }
private:
	ComPtr<ID3D11DomainShader> m_shader;
};
