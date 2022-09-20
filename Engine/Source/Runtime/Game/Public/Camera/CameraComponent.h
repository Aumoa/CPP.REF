// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CameraComponent.gen.h"

struct MinimalViewInfo;

SCLASS()
class GAME_API CameraComponent : public SceneComponent
{
	GENERATED_BODY()

public:
	SPROPERTY()
	float AspectRatio = 0.0f;

public:
	MinimalViewInfo ComputeViewInfo();
};