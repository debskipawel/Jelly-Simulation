#pragma once

#include "Scene.h"

class EntityFactory
{
public:
	EntityFactory() = delete;

	static SceneObject CreateCube(const D11Device& device, Scene& scene, float side = 1.0f);
	static SceneObject CreateDiagonal(const D11Device& device, Scene& scene, float side = 1.0f);
	static SceneObject CreateWorldGrid(const D11Device& device, Scene& scene);
};
