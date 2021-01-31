// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/Character.h"

class TH_API AMyCharacter : public ACharacter
{
public:
	using Super = ACharacter;
	using This = AMyCharacter;

private:
	Vector3 velocity;

	bool bWKeyDown : 1;
	bool bSKeyDown : 1;
	bool bAKeyDown : 1;
	bool bDKeyDown : 1;

public:
	AMyCharacter();
	~AMyCharacter();

	void Tick(Seconds deltaTime) override;

	void SetupPlayerInputComponent(InputComponent* inPlayerInput) override;

private:
	void UpdateMovement();
	void MovementKeyPressed(EKey inKey, EKeyEvent inKeyEvent);
};