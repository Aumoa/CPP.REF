// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "GameFramework/PlayerController.h"

class AChessPlayerController : public APlayerController
{
public:
	using Super = APlayerController;

public:
	AChessPlayerController();

	virtual void BeginPlay() override;

private:
	void SetupPlayerInput(InputComponent* inputComponent);
};