// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/CameraComponent.h"

#include "SceneRendering/MinimalViewInfo.h"

CameraComponent::CameraComponent() : Super()
	, aspectRatio(1.0f)
	, fov(0.25f * 3.14f)
{

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::CalcMinimalViewInfo(MinimalViewInfo& outViewInfo) const
{
	outViewInfo.FOV = fov;
	outViewInfo.AspectRatio = aspectRatio;
	outViewInfo.Location = ComponentLocation;
	outViewInfo.Rotation = ComponentRotation;
}