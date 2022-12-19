#pragma once

#include <vector>
#include <memory>
#include <functional>

#include <DirectX/Buffers/D11ConstantBuffer.h>
#include <Scene/SceneObject.h>

class D11Shader
{
public:
	inline void AddConstantBuffer(std::shared_ptr<D11ConstantBuffer> constantBuffer)
	{
		m_constantBuffers.push_back(constantBuffer);
	}

	inline void UpdateConstantBuffer(UINT bufferIndex, const void* data, size_t size)
	{
		m_constantBuffers[bufferIndex]->Update(data, size);
	}
	inline void AddTexture(ComPtr<ID3D11ShaderResourceView> srv)
	{
		m_textures.push_back(srv);
	}
	inline void UpdateTexture(UINT textureIndex, ComPtr<ID3D11ShaderResourceView> srv)
	{
		m_textures[textureIndex] = srv;
	}

	inline const auto& ConstantBuffers() const { return m_constantBuffers; }
	inline const auto& Textures() const { return m_textures; }
	std::vector<ID3D11Buffer*> RawConstantBuffers();
	std::vector<ID3D11ShaderResourceView*> RawTextures();

protected:
	explicit D11Shader() = default;

	std::vector<std::shared_ptr<D11ConstantBuffer>> m_constantBuffers;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_textures;
};
