// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/CameraComponent.h"

#include "SceneRendering/MinimalViewInfo.h"

GCameraComponent::GCameraComponent() : Super()
	, aspectRatio(0.0f)
	, fov(0.25f * 3.14f)
{
	Mobility = EComponentMobility::Movable;
}

GCameraComponent::~GCameraComponent()
{

}

void GCameraComponent::CalcCameraView(MinimalViewInfo& outViewInfo) const
{
	outViewInfo.FOV = fov;
	outViewInfo.AspectRatio = aspectRatio;
	outViewInfo.Location = ComponentLocation;
	outViewInfo.Rotation = ComponentRotation;
}