#include "pch.h"
#include "SceneObject.h"

#include <Scene/Scene.h>

SceneObject::SceneObject(Scene& scene)
    : m_scene(&scene), m_entityHandle(scene.m_registry.create())
{
}

SceneObject::SceneObject(Scene& scene, entt::entity entityHandle)
    : m_scene(&scene), m_entityHandle(entityHandle)
{
}

void SceneObject::Destroy()
{
    m_scene->m_registry.destroy(m_entityHandle);
}

