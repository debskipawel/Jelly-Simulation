#pragma once

#include <vector>
#include <SimpleMath.h>

#include <d3d11.h>

using namespace DirectX::SimpleMath;

static const std::vector<D3D11_INPUT_ELEMENT_DESC> g_cubePositionLayout
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

static const std::vector<Vector3> g_cubePositionVertices =
{
    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
};

static const std::vector<unsigned short> g_cubePositionIndices =
{
    0, 1, 0, 2, 2, 3, 1, 3,
    4, 5, 4, 6, 6, 7, 5, 7,
    0, 4, 1, 5, 2, 6, 3, 7
};
