// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Rotator.h"

struct GAME_API MinimalViewInfo
{
	float FOV;
	float AspectRatio;
	float NearPlane;
	float FarPlane;
	Vector3 Location;
	Quaternion Rotation;
	Matrix4x4 ViewProj;

	MinimalViewInfo();
	void Apply();
};