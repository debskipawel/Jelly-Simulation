#pragma once

#include "D11Shader.h"

class D11VertexShader : public D11Shader
{
public:
	explicit D11VertexShader(const D11Device& device, void* code, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
	{
		if (FAILED(device->CreateVertexShader(code, size, nullptr, m_shader.GetAddressOf())))
		{
			// TODO: log errors
			//printf("Error while creating vertex shader\n");
		}

		if (FAILED(device->CreateInputLayout(inputDesc.data(), static_cast<UINT>(inputDesc.size()), code, size, m_layout.GetAddressOf())))
		{
			// TODO: log errors
			//printf("Error while creating vertex shader input layout\n");
		}
	}

	inline ID3D11VertexShader* Shader() { return m_shader.Get(); }
	inline ID3D11InputLayout* GetLayout() { return m_layout.Get(); }

private:
	ComPtr<ID3D11InputLayout> m_layout;
	ComPtr<ID3D11VertexShader> m_shader;
};
