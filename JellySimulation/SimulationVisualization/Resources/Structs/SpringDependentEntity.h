#pragma once

#include <Scene/SceneObject.h>

#include <Components/SpringsComponent.h>
#include <Components/PhysicsComponent.h>
#include <Components/TransformComponent.h>

struct SpringDependentEntity
{
	SpringDependentEntity(SceneObject object, SpringsComponent& springs, PhysicsComponent& physics, TransformComponent& transform)
		: object(object), springs(springs), physics(physics), transform(transform)
	{
	}

	SceneObject object;

	SpringsComponent& springs;
	PhysicsComponent& physics;
	TransformComponent& transform;
};
