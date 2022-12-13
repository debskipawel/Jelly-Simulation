#pragma once

#include "Scene.h"

#include <utility>

class SceneObject
{
public:
	SceneObject() = default;
	SceneObject(Scene& scene);
	SceneObject(Scene& scene, entt::entity entityHandle);
	
	SceneObject(const SceneObject& object) = default;

	void Destroy();

	std::shared_ptr<AbstractCamera> GetCamera() const { return m_scene->Camera(); }

	template <typename T>
	inline T& GetComponent() const
	{
		return m_scene->m_registry.get<T>(m_entityHandle);
	}

	template <typename T> 
	inline bool HasComponent() const
	{
		return m_scene->m_registry.any_of<T>(m_entityHandle);
	}

	template <typename T, typename... Args> 
	inline T& AddComponent(Args&&... args)
	{
		return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
	}

protected:

	Scene* m_scene;
	entt::entity m_entityHandle;
};
