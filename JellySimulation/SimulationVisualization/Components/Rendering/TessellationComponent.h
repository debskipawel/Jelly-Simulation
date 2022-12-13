#pragma once

#include <DirectX/Shaders/D11HullShader.h>
#include <DirectX/Shaders/D11DomainShader.h>

#include <memory>

struct TessellationComponent
{
	TessellationComponent(std::shared_ptr<D11HullShader> hullShader, std::shared_ptr<D11DomainShader> domainShader)
		: HullShader(hullShader), DomainShader(domainShader)
	{}

	TessellationComponent() = default;
	TessellationComponent(const TessellationComponent& other) = default;

	std::shared_ptr<D11HullShader> HullShader;
	std::shared_ptr<D11DomainShader> DomainShader;
};
