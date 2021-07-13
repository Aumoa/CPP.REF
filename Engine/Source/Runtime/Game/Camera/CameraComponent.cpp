// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "CameraComponent.h"

using namespace std::chrono;

CameraComponent::CameraComponent()
{
}

MinimalViewInfo CameraComponent::GetViewInfo(duration<float> elapsedTime) const
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