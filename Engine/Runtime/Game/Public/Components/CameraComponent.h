// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

struct MinimalViewInfo;

class GAME_API CameraComponent : public SceneComponent
{
public:
	using Super = SceneComponent;
	using This = CameraComponent;

private:
	float aspectRatio;
	float fov;

public:
	CameraComponent();
	~CameraComponent() override;

	void CalcMinimalViewInfo(MinimalViewInfo& outViewInfo) const;
};