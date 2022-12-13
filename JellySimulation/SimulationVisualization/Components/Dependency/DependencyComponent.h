#pragma once

#include <Scene/SceneObject.h>

#include <vector>

struct DependencyComponent
{
	DependencyComponent()
		: dependentObjects() {}

	DependencyComponent(SceneObject obj)
		: dependentObjects()
	{
		dependentObjects.push_back(obj);
	}

	std::vector<SceneObject> dependentObjects;
};
