// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Pawn.h"

class GSceneComponent;

class GAME_API ACharacter : public APawn
{
public:
	using Super = APawn;
	using This = ACharacter;

private:
	GSceneComponent* capsuleComponent;

public:
	ACharacter();
	~ACharacter() override;
};