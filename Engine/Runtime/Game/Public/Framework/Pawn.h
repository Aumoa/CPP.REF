// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Actor.h"

class GAME_API APawn : public AActor
{
public:
	using Super = AActor;
	using This = APawn;

public:
	APawn();
	~APawn() override;
};