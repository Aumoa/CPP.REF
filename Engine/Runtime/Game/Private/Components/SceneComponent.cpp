// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/SceneComponent.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::Components;

SceneComponent::SceneComponent() : Super()
	, transform(Transform::Identity)
{

}

SceneComponent::~SceneComponent()
{

}

Transform SceneComponent::RelativeTransform_get() const
{
	return transform;
}

void SceneComponent::RelativeTransform_set(const Transform& value)
{
	transform = value;
}