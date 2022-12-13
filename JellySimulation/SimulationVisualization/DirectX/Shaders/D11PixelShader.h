#pragma once

#include "D11Shader.h"

class D11PixelShader : public D11Shader
{
public:
	explicit D11PixelShader(const D11Device& device, void* code, size_t size)
	{
		if (FAILED(device->CreatePixelShader(code, size, nullptr, m_shader.GetAddressOf())))
		{
			// TODO: log errors
			//printf("Error while creating pixel shader\n");
		}
	}

	inline ID3D11PixelShader* Shader() { return m_shader.Get(); }
private:
	ComPtr<ID3D11PixelShader> m_shader;
};
