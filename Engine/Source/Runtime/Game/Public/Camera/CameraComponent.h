// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MinimalViewInfo.h"

class GAME_API SCameraComponent : public SSceneComponent
{
	GENERATED_BODY(SCameraComponent)

public:
	SCameraComponent();

	MinimalViewInfo GetViewInfo(float elapsedTime);
};