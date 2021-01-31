// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Pawn.h"

#include "Key.h"

class FloatingPawnMovementComponent;

class GAME_API ASpectatorPawn : public APawn
{
public:
	using Super = APawn;
	using This = ASpectatorPawn;

private:
	bool bMoveForward : 1;
	bool bMoveBackward : 1;
	bool bMoveRight : 1;
	bool bMoveLeft : 1;

	FloatingPawnMovementComponent* movementComponent;

public:
	ASpectatorPawn();
	~ASpectatorPawn() override;

	void Tick(Seconds deltaTime) override;

	void SetupPlayerInputComponent(InputComponent* inPlayerInput) override;

	void MoveForward(float value);
	void MoveRight(float value);

private:
	void HandlePlayerInput(EKey inKey, EKeyEvent inEvent);
	void ProcessPlayerInput();
};