// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

class THSTORY_API ATHPlayerController : public APlayerController
{
	GENERATED_BODY(ATHPlayerController)

public:
	ATHPlayerController();

	virtual void Tick(float InDeltaTime) override;
};