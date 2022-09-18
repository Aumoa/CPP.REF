// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Camera/CameraComponent.h"
#include "Camera/MinimalViewInfo.h"
#include "CameraComponent.gen.cpp"

MinimalViewInfo CameraComponent::ComputeViewInfo()
{
	MinimalViewInfo Info;
	Info.AspectRatio = AspectRatio;
	Info.Location = GetComponentLocation();
	Info.Rotation = GetComponentRotation();
	return Info;
}