#include "pch.h"
#include "EntityFactory.h"

#include "SceneObject.h"

#include <Components/TransformComponent.h>
#include <Components/Rendering/RenderingComponent.h>
#include <Components/Rendering/TessellationComponent.h>
#include <Components/Rendering/DepthStateComponent.h>
#include <Components/Rendering/BlendStateComponent.h>

#include <DirectX/D11ShaderLoader.h>
#include <DirectX/D11Renderer.h>

#include <Resources/Meshes/BezierCube.h>
#include <Resources/Meshes/Cube.h>
#include <Resources/Meshes/CubeNormal.h>
#include <Resources/Meshes/WorldGrid.h>
#include <Resources/Meshes/Duck.h>

SceneObject EntityFactory::CreateCube(const D11Device& device, Scene& scene, float sideLength)
{
    auto cube = SceneObject(scene);

    auto positions = g_cubePositionVertices;
    for (size_t i = 0; i < g_cubePositionVertices.size(); i++)
    {
        positions[i] = positions[i] * sideLength;
    }
    auto vb = std::make_shared<D11VertexBuffer>(device, positions.size() * sizeof(Vector3), g_cubePositionLayout, positions.data());
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, g_cubePositionIndices.size() * sizeof(unsigned short), g_cubePositionIndices.data(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/mvp_pos_vs.hlsl", g_cubePositionLayout, { 4 * sizeof(Matrix) });
    auto ps = D11ShaderLoader::PSLoad(device, L"../shaders_bin/solid_white_ps.hlsl");

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
    cube.AddComponent<TransformComponent>();

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

SceneObject EntityFactory::CreateBezierCube(const D11Device& device, Scene& scene)
{
    auto cube = SceneObject(scene);
    auto indices = GetBezierCubeIndices();
    std::vector<Vector3> vertices(64);

    auto vb = std::make_shared<D11VertexBuffer>(device, vertices.size() * sizeof(Vector3), g_bezierCubePositionLayout, vertices.data());
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/mvp_pos_vs.hlsl", g_bezierCubePositionLayout, { 4 * sizeof(Matrix) });
    auto ps = D11ShaderLoader::PSLoad(device, L"../shaders_bin/solid_color_ps.hlsl", { sizeof(Vector4) });

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
            Vector4 color = { 0.0f, 0.5f, 0.1f, 0.5f };

            rendering.VertexShader->UpdateConstantBuffer(0, buf, 4 * sizeof(Matrix));
            rendering.PixelShader->UpdateConstantBuffer(0, &color, sizeof(Vector4));
        }
    );
    cube.AddComponent<TransformComponent>();

    return cube;
}

SceneObject EntityFactory::CreateShadedBezierCube(const D11Device& device, Scene& scene)
{
    auto cube = SceneObject(scene);
    auto& indices = g_bezierCubeSidesIndices;
    std::vector<Vector3> vertices(64);

    auto vb = std::make_shared<D11VertexBuffer>(device, vertices.size() * sizeof(Vector3), g_bezierCubePositionLayout, vertices.data());
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data(), D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST);

    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/mvp_pos_vs.hlsl", g_bezierCubePositionLayout, { 4 * sizeof(Matrix) });
    auto ps = D11ShaderLoader::PSLoad(device, L"../shaders_bin/solid_color_ps.hlsl", { sizeof(Vector4) });

    auto hs = D11ShaderLoader::HSLoad(device, L"../shaders_bin/bezier_surface_hs.hlsl");
    auto ds = D11ShaderLoader::DSLoad(device, L"../shaders_bin/bezier_surface_ds.hlsl");

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
            Vector4 color = { 0.0f, 0.1f, 0.5f, 0.5f };

            rendering.VertexShader->UpdateConstantBuffer(0, buf, 4 * sizeof(Matrix));
            rendering.PixelShader->UpdateConstantBuffer(0, &color, sizeof(Vector4));
        }
    );
    cube.AddComponent<TessellationComponent>(hs, ds);
    cube.AddComponent<TransformComponent>();
    cube.AddComponent<DepthStateComponent>(nullptr);

    return cube;
}

SceneObject EntityFactory::CreateDuck(const D11Device& device, Scene& scene, const std::wstring& meshPath, std::vector<SpringDependentEntity>& controlPoints)
{
    auto sceneObject = SceneObject(scene);
    auto [vertices, indices] = ReadMeshFile(meshPath);
    auto vb = std::make_shared<D11VertexBuffer>(device, vertices.size() * sizeof(VertexPositionNormalTex), g_duckLayout, vertices.data());
    auto ib = std::make_shared<D11IndexBuffer>(device, DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data(), D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    auto vs = D11ShaderLoader::VSLoad(device, L"../shaders_bin/duck_vs.hlsl", g_duckLayout, { 4 * sizeof(Matrix), 64*sizeof(Vector4)});
    auto ps = D11ShaderLoader::PSLoad(device, L"../shaders_bin/duck_ps.hlsl", { sizeof(Vector4) });

    auto texture = device.CreateShaderResourceViewFromFile(L"..\\Resources\\MeshesFiles\\ducktex.jpg");
    ps->AddTexture(texture);

    sceneObject.AddComponent<BlendStateComponent>(device);
    sceneObject.AddComponent<DepthStateComponent>(device);
    sceneObject.AddComponent<RenderingComponent>(
        vb, ib, vs, ps,
        [&](const D11Renderer& renderer, SceneObject object)
        {
            auto view = object.GetCamera()->GetViewMatrix();
            Matrix buf[] = {
                Matrix::CreateScale(0.005) * Matrix::CreateRotationX(XM_PIDIV2) * Matrix::CreateTranslation(0,0,-0.3),
                view,
                view.Invert(),
                renderer.GetProjectionMatrix()
            };

            rendering.VertexShader->UpdateConstantBuffer(0, buf, 4 * sizeof(Matrix));

            Vector4 points[64];
            for (int i = 0; i < 64; i++)
            {
                auto& point = controlPoints[i];
                auto position = point.transform.Position;
                auto position4 = Vector4(position.x, position.y, position.z, 1.0f);

                points[i] = position4;
            }
            rendering.VertexShader->UpdateConstantBuffer(1, points, 64 * sizeof(Vector4));


            auto lightPosition = Vector4(10, 10, 10,1.0f);

            rendering.PixelShader->UpdateConstantBuffer(0, &lightPosition, sizeof(Vector4));
        }
    );

    return sceneObject;
}

std::tuple<std::vector<VertexPositionNormalTex>, std::vector<unsigned short>> EntityFactory::ReadMeshFile(const std::wstring& meshPath)
{
    //File format for VN vertices and IN indices (IN divisible by 3, i.e. IN/3 triangles):
    //VN IN
    //pos.x pos.y pos.z norm.x norm.y norm.z tex.x tex.y [VN times, i.e. for each vertex]
    //t.i1 t.i2 t.i3 [IN/3 times, i.e. for each triangle]

    std::ifstream input;
    // In general we shouldn't throw exceptions on end-of-file,
    // however, in case of this file format if we reach the end
    // of a file before we read all values, the file is
    // ill-formated and we would need to throw an exception anyway
    input.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
    input.open(meshPath);

    int verticesCount;
    input >> verticesCount;
    std::vector<VertexPositionNormalTex> verts(verticesCount);

    for (int i = 0; i < verticesCount; i++)
    {
        input >> verts[i].position.x >> verts[i].position.y >> verts[i].position.z
            >> verts[i].normal.x >> verts[i].normal.y >> verts[i].normal.z
            >> verts[i].tex.x >> verts[i].tex.y;
    }

    int trianglesCount;
    input >> trianglesCount;
    std::vector<unsigned short> tri(trianglesCount * 3);
    for (auto i = 0; i < trianglesCount; ++i)
        input >> tri[i * 3] >> tri[i * 3 + 1] >> tri[i * 3 + 2];

    return std::make_tuple(verts, tri);
}
