// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Controller.h"

class GAME_API APlayerController : public AController
{
public:
	using Super = AController;
	using This = APlayerController;

public:
	APlayerController();
	~APlayerController() override;
};