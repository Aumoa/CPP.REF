// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Info.h"

#include "TSubclassOf.h"

class APlayerController;
class GLevel;

class GAME_API AGameModeBase : public AInfo
{
public:
	using Super = AInfo;
	using This = AGameModeBase;

public:
	AGameModeBase();
	~AGameModeBase() override;

	TSubclassOf<APlayerController> PlayerControllerClass;
	TSubclassOf<GLevel> StartLevelClass;
};