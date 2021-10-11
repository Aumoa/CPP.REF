// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct SceneViewScope
{
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjMatrix;

	static SceneViewScope InitFromCameraView(const Vector3& InLocation, const Quaternion& InRotation, Radians InFOV, float InAspectRatio, float InMinDepth, float InMaxDepth)
	{
		constexpr static Vector3 FrontDirection = Vector3(1.0f, 0, 0);
		constexpr static Vector3 UpDirection = Vector3(0, 1.0f, 0);

		SceneViewScope ViewScope;
		ViewScope.ViewMatrix = Matrix4x4::LookToLH(InLocation, InRotation.RotateVector(FrontDirection), InRotation.RotateVector(UpDirection));
		ViewScope.ProjMatrix = Matrix4x4::PerspectiveFovLH(InFOV, InAspectRatio, InMinDepth, InMaxDepth);
		return ViewScope;
	}
};