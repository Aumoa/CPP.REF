// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Camera/CameraComponent.h"
#include "GameStructures.h"

SCameraComponent::SCameraComponent()
{
}

MinimalViewInfo SCameraComponent::GetViewInfo(std::chrono::duration<float> elapsedTime) const
{
	MinimalViewInfo vinfo;
	vinfo.bInit = true;
	vinfo.Location = GetComponentLocation();
	vinfo.Rotation = GetComponentRotation();
	vinfo.NearPlane = 0.1f;
	vinfo.FarPlane = 1000.0f;
	vinfo.FOVAngle = 45.0f;
	return vinfo;
}