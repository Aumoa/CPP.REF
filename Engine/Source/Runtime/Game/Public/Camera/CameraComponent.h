// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

struct MinimalViewInfo;

class GAME_API SCameraComponent : public SSceneComponent
{
	GENERATED_BODY(SCameraComponent)

public:
	SCameraComponent();

	MinimalViewInfo GetViewInfo(std::chrono::duration<float> elapsedTime) const;
};