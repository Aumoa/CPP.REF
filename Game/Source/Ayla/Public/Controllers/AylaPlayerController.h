// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AylaPlayerController.generated.h"

SCLASS()
class AYLA_API AAylaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAylaPlayerController();

protected:
	virtual void Tick(const TimeSpan& InDeltaTime) override;
};