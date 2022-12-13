#pragma once

#include <entt/entt.hpp>

#include <Camera/AbstractCamera.h>
#include <DirectX/D11Device.h>

class SceneObject;

template <typename... Args> class SceneIterator;

class Scene
{
public:
	Scene();

	std::shared_ptr<AbstractCamera> Camera() const { return m_activeCamera; }

	template <typename... Args>
	SceneIterator<Args...> Begin()
	{
		return SceneIterator<Args...>(*this);
	}

	template <typename... Args>
	SceneIterator<Args...> End()
	{
		return SceneIterator<Args...>::End(*this);
	}

protected:
	entt::registry m_registry;

	std::shared_ptr<AbstractCamera> m_activeCamera;

	friend class SceneObject;
	template<typename... Args> friend class SceneIterator;
};
