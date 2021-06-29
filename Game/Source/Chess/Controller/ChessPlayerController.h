// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "GameFramework/PlayerController.h"

class PrimitiveComponent;

class AChessPlayerController : public APlayerController
{
public:
	using Super = APlayerController;

private:
	PrimitiveComponent* _indicator = nullptr;

public:
	AChessPlayerController();

	virtual void BeginPlay() override;

private:
	void SetupPlayerInput(InputComponent* inputComponent);
};