#pragma once

#include "SceneObject.h"

#include <entt/entt.hpp>

template <typename... Args>
class SceneIterator
{
public:
	SceneIterator(Scene& scene);
	SceneIterator(const SceneIterator& other) = default;

	bool operator==(const SceneIterator& other);
	bool operator!=(const SceneIterator& other);

	SceneIterator<Args...>& operator++();
	SceneIterator<Args...> operator++(int);

	void Reset();
	void GoNext();
	SceneObject Get();

	static SceneIterator<Args...> End(Scene& scene);

protected:
	bool HasNext();

	Scene& m_scene;

	entt::basic_view<entt::entity, entt::get_t<Args...>, entt::exclude_t<>, void> m_collection;
	int m_index;
};

template<typename... Args>
inline SceneIterator<Args...>::SceneIterator(Scene& scene)
	: m_scene(scene)
{
	m_collection = scene.m_registry.view<Args...>();
	m_index = 0;
}

template<typename... Args >
inline bool SceneIterator<Args...>::operator==(const SceneIterator<Args...>& other)
{
	return &m_scene == &other.m_scene && m_index == other.m_index;
}

template<typename... Args>
inline bool SceneIterator<Args...>::operator!=(const SceneIterator<Args...>& other)
{
	return !(*this == other);
}

template<typename... Args>
inline SceneIterator<Args...>& SceneIterator<Args...>::operator++()
{
	GoNext();
	return *this;
}

template<typename... Args>
inline SceneIterator<Args...> SceneIterator<Args...>::operator++(int)
{
	GoNext();
	return *this;
}

template<typename... Args>
inline void SceneIterator<Args...>::Reset()
{
	m_index = 0;
}

template<typename... Args>
inline void SceneIterator<Args...>::GoNext()
{
	if (HasNext())
	{
		m_index++;
	}
}

template<typename... Args>
inline SceneObject SceneIterator<Args...>::Get()
{
	auto entity = m_collection[m_index];
	return SceneObject(m_scene, entity);
}

template<typename... Args>
inline SceneIterator<Args...> SceneIterator<Args...>::End(Scene& scene)
{
	auto it = SceneIterator<Args...>(scene);
	it.m_index = it.m_collection.size();

	return it;
}

template<typename... Args>
inline bool SceneIterator<Args...>::HasNext()
{
	return m_index <= m_collection.size() - 1;
}
