#pragma once

#include <DirectX/Buffers/D11VertexBuffer.h>
#include <DirectX/Buffers/D11IndexBuffer.h>
#include <DirectX/Shaders/D11VertexShader.h>
#include <DirectX/Shaders/D11PixelShader.h>

#include <DirectX/D11Renderer.h>

#include <memory>

struct RenderingComponent
{
	RenderingComponent(
		std::shared_ptr<D11VertexBuffer> vertexBuffer,
		std::shared_ptr<D11IndexBuffer> indexBuffer,
		std::shared_ptr<D11VertexShader> vertexShader,
		std::shared_ptr<D11PixelShader> pixelShader,
		std::function<void(const D11Renderer&, SceneObject)> onRenderingHandle = [](const D11Renderer& device, SceneObject) {}
	) 
		: VertexBuffer(vertexBuffer), IndexBuffer(indexBuffer), VertexShader(vertexShader), PixelShader(pixelShader),
		OnRenderingHandle(onRenderingHandle), ShouldRender(true)
	{
	}

	RenderingComponent() = default;
	RenderingComponent(const RenderingComponent& other) = default;

	bool ShouldRender;

	std::shared_ptr<D11VertexBuffer> VertexBuffer;
	std::shared_ptr<D11IndexBuffer> IndexBuffer;
	std::shared_ptr<D11VertexShader> VertexShader;
	std::shared_ptr<D11PixelShader> PixelShader;

	std::function<void(const D11Renderer&, SceneObject)> OnRenderingHandle;
};
