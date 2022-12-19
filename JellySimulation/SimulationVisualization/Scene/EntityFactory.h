#pragma once

#include "Scene.h"
#include <Resources/Meshes/Duck.h>
#include <Resources/Structs/SpringDependentEntity.h>

class EntityFactory
{
public:
	EntityFactory() = delete;

	static SceneObject CreateCube(const D11Device& device, Scene& scene, float sideLength);
	static SceneObject CreateDiagonal(const D11Device& device, Scene& scene, float side = 1.0f);
	static SceneObject CreateWorldGrid(const D11Device& device, Scene& scene);
	static SceneObject CreateBezierCube(const D11Device& device, Scene& scene);
	static SceneObject CreateDuck(const D11Device& device, Scene& scene, const std::wstring& meshPath, std::vector<SpringDependentEntity>& controlPoints);

private:
	static std::tuple<std::vector<VertexPositionNormalTex>, std::vector<unsigned short>> ReadMeshFile(const std::wstring& meshPath);
};
