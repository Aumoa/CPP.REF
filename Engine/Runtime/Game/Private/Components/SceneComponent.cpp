// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/SceneComponent.h"

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