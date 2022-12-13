#pragma once

#include <DirectX/Shaders/D11VertexShader.h>
#include <DirectX/Shaders/D11PixelShader.h>
#include <DirectX/Shaders/D11GeometryShader.h>
#include <DirectX/Shaders/D11HullShader.h>
#include <DirectX/Shaders/D11DomainShader.h>

#include <map>
#include <string>
#include <functional>

class D11ShaderLoader
{
public:
	D11ShaderLoader() = delete;

	static void Clear();

	static std::shared_ptr<D11VertexShader> VSLoad(
		const D11Device& device, 
		std::wstring shaderPath, 
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
		const std::vector<size_t>& constants = {},
		std::string mainFunName = "main"
	);

	static std::shared_ptr<D11HullShader> HSLoad(
		const D11Device& device, 
		std::wstring shaderPath, 
		const std::vector<size_t>& constants = {}, 
		std::string mainFunName = "main"
	);

	static std::shared_ptr<D11DomainShader> DSLoad(
		const D11Device& device, 
		std::wstring shaderPath, 
		const std::vector<size_t>& constants = {}, 
		std::string mainFunName = "main"
	);

	static std::shared_ptr<D11GeometryShader> GSLoad(
		const D11Device& device, 
		std::wstring shaderPath, 
		const std::vector<size_t>& constants = {},
		std::string mainFunName = "main"
	);
	
	static std::shared_ptr<D11PixelShader> PSLoad(
		const D11Device& device, 
		std::wstring shaderPath, 
		const std::vector<size_t>& constants = {}, 
		std::string mainFunName = "main"
	);

protected:
	static std::map<std::wstring, std::shared_ptr<D11VertexShader>> m_vertexShaders;
	static std::map<std::wstring, std::shared_ptr<D11PixelShader>> m_pixelShaders;
	static std::map<std::wstring, std::shared_ptr<D11HullShader>> m_hullShaders;
	static std::map<std::wstring, std::shared_ptr<D11DomainShader>> m_domainShaders;
	static std::map<std::wstring, std::shared_ptr<D11GeometryShader>> m_geometryShaders;
};
