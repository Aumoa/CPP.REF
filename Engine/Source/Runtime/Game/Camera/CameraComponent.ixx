// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:CameraComponent;

import :SceneComponent;
import :MinimalViewInfo;

using namespace std::chrono;

export class CameraComponent : public SceneComponent
{
public:
	using Super = SceneComponent;

public:
	CameraComponent();

	MinimalViewInfo GetViewInfo(duration<float> elapsedTime) const;
};