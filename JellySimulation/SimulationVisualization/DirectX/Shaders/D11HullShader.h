#pragma once

#include "D11Shader.h"

class D11HullShader : public D11Shader
{
public:
	explicit D11HullShader(const D11Device& device, void* code, size_t size)
	{
		if (FAILED(device->CreateHullShader(code, size, nullptr, m_shader.GetAddressOf())))
		{
			// TODO: log errors
			//printf("Error while creating hull shader\n");
		}
	}

	inline ID3D11HullShader* Shader() { return m_shader.Get(); }
private:
	ComPtr<ID3D11HullShader> m_shader;
};
