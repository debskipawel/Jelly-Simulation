#include "pch.h"
#include "Scene.h"
#include "SceneObject.h"
#include "SceneIterator.h"

#include <Camera/OrbitCamera.h>

#include <Components/TransformComponent.h>
#include <Components/Rendering/RenderingComponent.h>

#include <DirectX/D11ShaderLoader.h>
#include <DirectX/D11Renderer.h>

Scene::Scene()
    : m_registry(), m_activeCamera(std::make_shared<OrbitCamera>(Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f, 15.0f, 5.0f))
{
}


