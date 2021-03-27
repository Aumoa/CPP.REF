// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

struct MinimalViewInfo;

class GAME_API GCameraComponent : public GSceneComponent
{
public:
	using Super = GSceneComponent;
	using This = GCameraComponent;

private:
	float aspectRatio;
	float fov;

public:
	GCameraComponent();
	~GCameraComponent() override;

	void CalcCameraView(MinimalViewInfo& outViewInfo) const;
};