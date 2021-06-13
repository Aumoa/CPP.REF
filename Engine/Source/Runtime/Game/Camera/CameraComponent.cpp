// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import std.core;

using namespace std::chrono;

CameraComponent::CameraComponent()
{
}

MinimalViewInfo CameraComponent::GetViewInfo(duration<float> elapsedTime) const
{
	MinimalViewInfo vinfo;
	vinfo.Location = GetComponentLocation();
	vinfo.Rotation = GetComponentRotation();
	return vinfo;
}