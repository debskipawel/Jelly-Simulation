#include "pch.h"
#include "EntityFactory.h"

#include "SceneObject.h"

#include <Components/TransformComponent.h>
#include <Components/Rendering/RenderingComponent.h>
#include <Components/Rendering/DepthStateComponent.h>

#include <DirectX/D11ShaderLoader.h>
#include <DirectX/D11Renderer.h>

#include <Resources/Meshes/Cube.h>
#include <Resources/Meshes/CubeNormal.h>
#include <Resources/Meshes/WorldGrid.h>

SceneObject EntityFactory::CreateCube(const D11Device& device, Scene& scene, float side)
{
    auto cube = SceneObject(scene);

    auto transformMatrix = Matrix::CreateTranslation(0.5, 0.5, 0.5) * Matrix::CreateScale(side) * Matrix::CreateRotationZ(XM_PIDIV4)* Matrix::CreateRotationX(acosf(1.0f / sqrtf(3))) ;
    std::vector<Vector3> transformedVertices(g_cubeNormalVertices.size());

    std::transform(g_cubeNormalVertices.begin(), g_cubeNormalVertices.end(), transformedVertices.begin(),
        [transformMatrix](const Vector3& v)
        {
            return Vector3::Transform(v, transformMatrix);
        }
    );

    auto vb = std::make_shared<D11VertexBuffer>(device, transformedVertices.size() * sizeof(Vector3), g_cubeNormalLayout, transformedVertices.data());
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, g_cubeNormalIndices.size() * sizeof(unsigned short), g_cubeNormalIndices.data());

    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/mvp_vs.hlsl", g_cubeNormalLayout, { 4 * sizeof(Matrix) });
    auto ps = D11ShaderLoader::PSLoad(device, L"../shaders_bin/lighting_ps.hlsl");

    cube.AddComponent<RenderingComponent>(
        vb, ib, vs, ps,
        [](const D11Renderer& renderer, SceneObject object)
        {
            auto& rendering = object.GetComponent<RenderingComponent>();

            auto view = object.GetCamera()->GetViewMatrix();
            Matrix buf[] = {
                object.GetComponent<TransformComponent>(),
                view,
                view.Invert(),
                renderer.GetProjectionMatrix()
            };

            rendering.VertexShader->UpdateConstantBuffer(0, buf, 4 * sizeof(Matrix));
        }
    );
    cube.AddComponent<TransformComponent>(Vector3{ 0.0f, 0.0f, side * sqrtf(3) / 2.0f }, Quaternion::Identity, Vector3::One);

    return cube;
}

SceneObject EntityFactory::CreateDiagonal(const D11Device& device, Scene& scene, float side)
{
    auto diagonal = SceneObject(scene);
    Vector3 vertices[] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }};
    unsigned short indices[] = { 0, 1 };

    auto vb = std::make_shared<D11VertexBuffer>(device, 4 * sizeof(Vector3), g_cubePositionLayout, vertices);
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, 2 * sizeof(unsigned short), indices, D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/mvp_pos_vs.hlsl", g_cubePositionLayout, { 4 * sizeof(Matrix) });
    auto ps = D11ShaderLoader::PSLoad(device, L"../shaders_bin/solid_color_ps.hlsl", { sizeof(Vector4) });

    diagonal.AddComponent<RenderingComponent>(
        vb, ib, vs, ps,
        [](const D11Renderer& renderer, SceneObject object)
        {
            auto& rendering = object.GetComponent<RenderingComponent>();

            auto view = object.GetCamera()->GetViewMatrix();
            Matrix buf[] = {
                object.GetComponent<TransformComponent>(),
                view,
                view.Invert(),
                renderer.GetProjectionMatrix()
            };

            Vector4 color = { 0.5f, 0.5f, 0.0f, 0.0f };

            rendering.VertexShader->UpdateConstantBuffer(0, buf, 4 * sizeof(Matrix));
            rendering.PixelShader->UpdateConstantBuffer(0, &color, sizeof(Vector4));
        }
    );
    diagonal.AddComponent<TransformComponent>(Vector3{ 0.0f, 0.0f, 0.0f }, Quaternion::Identity, Vector3{ 1.0f, 1.0f, side * sqrtf(3) });

    return diagonal;
}

SceneObject EntityFactory::CreateWorldGrid(const D11Device& device, Scene& scene)
{
    auto grid = SceneObject(scene);

    auto vb = std::make_shared<D11VertexBuffer>(device, g_worldGridVertices.size() * sizeof(Vector3), g_worldGridLayout, g_worldGridVertices.data());
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, g_worldGridIndices.size() * sizeof(unsigned short), g_worldGridIndices.data());

    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/pos_unproject_vs.hlsl", g_worldGridLayout, { 2 * sizeof(Matrix) });
    auto ps = D11ShaderLoader::PSLoad(device, L"../shaders_bin/grid_ps.hlsl", { 2 * sizeof(Matrix) });

    grid.AddComponent<DepthStateComponent>(device);
    grid.AddComponent<RenderingComponent>(
        vb, ib, vs, ps,
        [](const D11Renderer& renderer, SceneObject object)
        {
            auto& rendering = object.GetComponent<RenderingComponent>();

            Matrix cb0[] =
            {
                object.GetCamera()->GetViewMatrix(),
                renderer.GetProjectionMatrix(),
            };

            Matrix cb1[] =
            {
                cb0[0].Invert(),
                cb0[1].Invert(),
            };

            rendering.VertexShader->UpdateConstantBuffer(0, cb1, 2 * sizeof(Matrix));
            rendering.PixelShader->UpdateConstantBuffer(0, cb0, 2 * sizeof(Matrix));
        }
    );

    return grid;
}
