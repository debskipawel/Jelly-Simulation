#include "pch.h"
#include "D11ShaderLoader.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

std::vector<BYTE> LoadByteCode(const std::wstring& shaderPath)
{
	std::wcout << L"Trying to read shader from: " << shaderPath << std::endl;

	std::ifstream sIn(shaderPath, std::ios::in | std::ios::binary);

	sIn.seekg(0, std::ios::end);
	auto byteCodeLength = sIn.tellg();
	sIn.seekg(0, std::ios::beg);

	std::vector<BYTE> byteCode(byteCodeLength);

	sIn.read(reinterpret_cast<char*>(byteCode.data()), byteCodeLength);
	sIn.close();

	return byteCode;
}

bool SaveToFile(const void* data, size_t size, const std::wstring& path)
{
	try
	{
		std::ofstream sOut(path, std::ios::out | std::ios::binary);
		sOut.write(reinterpret_cast<const char*>(data), size);
		sOut.close();

		return true;
	}
	catch (std::exception)
	{
		return false;
	}
}

HRESULT CompileShader(
	_In_ LPCWSTR srcFile, 
	_In_ LPCSTR entryPoint, 
	_In_ LPCSTR profile, 
	_Outptr_ ID3DBlob** blob
)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
		{
			shaderBlob->Release();
		}

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

template<typename T>
std::shared_ptr<T> ShaderLoad(
	std::map<std::wstring, std::shared_ptr<T>>& map,
	const D11Device& device,
	std::wstring shaderPath,
	const std::vector<size_t>& constants,
	std::string mainFunName,
	std::string shaderVersion
)
{
	auto result = map.find(shaderPath);

	if (result != map.end())
	{
		return result->second;
	}

	std::shared_ptr<T> shader;

	if (shaderPath.substr(shaderPath.length() - 4) == L".cso")
	{
		auto bytecode = LoadByteCode(shaderPath);
		shader = std::make_shared<T>(device, bytecode.data(), bytecode.size());
	}
	else
	{
		auto shaderName = shaderPath.substr(0, shaderPath.find_last_of(L'.'));
		auto compiledShaderName = shaderName + L".cso";

		try
		{
			// try loading compiled shader if exists
			auto bytecode = LoadByteCode(compiledShaderName);
			shader = std::make_shared<T>(device, bytecode.data(), bytecode.size());
		}
		catch (std::exception e)
		{
			ID3DBlob* blob = nullptr;
			auto hr = CompileShader(shaderPath.c_str(), mainFunName.c_str(), shaderVersion.c_str(), &blob);

			shader = std::make_shared<T>(device, blob->GetBufferPointer(), blob->GetBufferSize());

			SaveToFile(blob->GetBufferPointer(), blob->GetBufferSize(), compiledShaderName);

			blob->Release();
		}
	}

	for (auto& size : constants)
	{
		auto cb = std::make_shared<D11ConstantBuffer>(device, size);
		shader->AddConstantBuffer(cb);
	}

	map.insert(std::make_pair(shaderPath, shader));

	return shader;
}

void D11ShaderLoader::Clear()
{
	m_vertexShaders.clear();
	m_pixelShaders.clear();
	m_domainShaders.clear();
	m_hullShaders.clear();
	m_geometryShaders.clear();
}

std::shared_ptr<D11VertexShader> D11ShaderLoader::VSLoad(const D11Device& device, std::wstring shaderPath, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const std::vector<size_t>& constants, std::string mainFunName)
{
	auto result = m_vertexShaders.find(shaderPath);

	if (result != m_vertexShaders.end())
	{
		return result->second;
	}

	std::shared_ptr<D11VertexShader> shader;

	if (shaderPath.substr(shaderPath.length() - 4) == L".cso")
	{
		auto bytecode = LoadByteCode(shaderPath);
		shader = std::make_shared<D11VertexShader>(device, bytecode.data(), bytecode.size(), layout);
	}
	else
	{
		auto shaderName = shaderPath.substr(0, shaderPath.find_last_of(L'.'));
		auto compiledShaderName = shaderName + L".cso";

		try
		{
			auto bytecode = LoadByteCode(compiledShaderName);
			shader = std::make_shared<D11VertexShader>(device, bytecode.data(), bytecode.size(), layout);
		}
		catch (std::exception e)
		{
			ID3DBlob* vsBlob = nullptr;
			auto hr = CompileShader(shaderPath.c_str(), mainFunName.c_str(), "vs_4_0_level_9_1", &vsBlob);
			shader = std::make_shared<D11VertexShader>(device, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), layout);
			vsBlob->Release();

			SaveToFile(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), compiledShaderName);
		}
	}

	for (auto& size : constants)
	{
		auto cb = std::make_shared<D11ConstantBuffer>(device, size);
		shader->AddConstantBuffer(cb);
	}

	m_vertexShaders.insert(std::make_pair(shaderPath, shader));

	return shader;
}

std::shared_ptr<D11HullShader> D11ShaderLoader::HSLoad(const D11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
{
	return ShaderLoad(m_hullShaders, device, shaderPath, constants, mainFunName, "hs_4_0_level_9_1");
}

std::shared_ptr<D11DomainShader> D11ShaderLoader::DSLoad(const D11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
{
	return ShaderLoad(m_domainShaders, device, shaderPath, constants, mainFunName, "ds_4_0_level_9_1");
}

std::shared_ptr<D11GeometryShader> D11ShaderLoader::GSLoad(const D11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
{
	return ShaderLoad(m_geometryShaders, device, shaderPath, constants, mainFunName, "gs_4_0_level_9_1");
}

std::shared_ptr<D11PixelShader> D11ShaderLoader::PSLoad(const D11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
{
	return ShaderLoad(m_pixelShaders, device, shaderPath, constants, mainFunName, "ps_4_0_level_9_1");
}

std::map<std::wstring, std::shared_ptr<D11VertexShader>> D11ShaderLoader::m_vertexShaders;
std::map<std::wstring, std::shared_ptr<D11PixelShader>> D11ShaderLoader::m_pixelShaders;
std::map<std::wstring, std::shared_ptr<D11HullShader>> D11ShaderLoader::m_hullShaders;
std::map<std::wstring, std::shared_ptr<D11DomainShader>> D11ShaderLoader::m_domainShaders;
std::map<std::wstring, std::shared_ptr<D11GeometryShader>> D11ShaderLoader::m_geometryShaders;
