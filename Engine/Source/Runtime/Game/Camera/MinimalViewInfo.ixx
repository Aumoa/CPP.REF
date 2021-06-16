// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:MinimalViewInfo;

import SC.Runtime.Core;

export struct MinimalViewInfo
{
	Vector3 Location;
	Quaternion Rotation;
	float AspectRatio = 1.0f;
};