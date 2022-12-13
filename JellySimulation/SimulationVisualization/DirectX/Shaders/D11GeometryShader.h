#pragma once

#include "D11Shader.h"

class D11GeometryShader : public D11Shader
{
public:
	explicit D11GeometryShader(const D11Device& device, void* code, size_t size)
	{
		if (FAILED(device->CreateGeometryShader(code, size, nullptr, m_shader.GetAddressOf())))
		{
			// TODO: log errors
			//printf("Error while creating geometry shader\n");
		}
	}

	inline ID3D11GeometryShader* Shader() { return m_shader.Get(); }
private:
	ComPtr<ID3D11GeometryShader> m_shader;
};
