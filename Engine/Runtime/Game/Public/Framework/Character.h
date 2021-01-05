// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Pawn.h"

class SceneComponent;

class GAME_API ACharacter : public APawn
{
public:
	using Super = APawn;
	using This = ACharacter;

private:
	SceneComponent* capsuleComponent;

public:
	ACharacter();
	~ACharacter() override;
};