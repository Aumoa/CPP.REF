// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameEngine.generated.h"

SCLASS()
class GAME_API GameEngine : public Engine
{
	GENERATED_BODY()

public:
	SCONSTRUCTOR()
	GameEngine();

	virtual void Init() override;
};