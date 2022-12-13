#include "pch.h"
#include "ParticleSystem.h"

#include <Components/Particles/ParticleEmitterComponent.h>
#include <Components/Rendering/RenderingComponent.h>

#include <DirectX/D11ShaderLoader.h>
#include <DirectX/Buffers/D11VertexBuffer.h>
#include <DirectX/Buffers/D11IndexBuffer.h>

#include <Scene/SceneIterator.h>

static const std::vector<D3D11_INPUT_ELEMENT_DESC> g_particleBufferLayout
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "START_TIME", 0, DXGI_FORMAT_R32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "END_TIME", 0, DXGI_FORMAT_R32_FLOAT, 0, sizeof(Vector3) + sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "PERCENTAGE", 0, DXGI_FORMAT_R32_FLOAT, 0, sizeof(Vector3) + 2 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ParticleSystem::ParticleSystem(Scene& scene)
    : m_scene(&scene)
{
}

SceneObject ParticleSystem::AddInstance(const D11Device& device, std::wstring pixelShaderName, D3D11_PRIMITIVE_TOPOLOGY topology)
{
    SceneObject particle(*m_scene);

    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/particle_vs.hlsl", g_particleBufferLayout, { 2 * sizeof(Matrix) });
    auto ps = D11ShaderLoader::PSLoad(device, pixelShaderName);
    auto vb = std::make_shared<D11VertexBuffer>(device, 64 * sizeof(Particle), g_particleBufferLayout);
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 64, nullptr, topology);

    auto& particleComponent = particle.AddComponent<ParticleEmitterComponent>();
    auto& renderingComponent = particle.AddComponent<RenderingComponent>(
        vb, ib, vs, ps,
        [](const D11Renderer& renderer, SceneObject object)
        {
            auto& rendering = object.GetComponent<RenderingComponent>();

            Matrix buf[] = {
                object.GetCamera()->GetViewMatrix(),
                renderer.GetProjectionMatrix()
            };

            auto& vs = rendering.VertexShader;
            vs->UpdateConstantBuffer(0, buf, 2 * sizeof(Matrix));
        }
    );

    return particle;
}

void ParticleSystem::Update(float dt)
{
    using T = ParticleEmitterComponent;

    for (auto it = m_scene->Begin<T>(); it != m_scene->End<T>(); it++)
    {
        auto entity = it.Get();
        
        auto& particleComponent = entity.GetComponent<T>();
        particleComponent.Update(dt);

        auto& particles = particleComponent.Particles();
        auto& rendering = entity.GetComponent<RenderingComponent>();
        
        auto& vb = rendering.VertexBuffer;
        auto& ib = rendering.IndexBuffer;

        vb->Update(particles.data(), particles.size() * sizeof(Particle));

        std::vector<unsigned short> indices(particles.size());
        
        for (int i = 0; i < particles.size(); i++)
        {
            indices[i] = i;
        }

        ib->Update(indices.data(), particles.size() * sizeof(unsigned short));
    }
}
