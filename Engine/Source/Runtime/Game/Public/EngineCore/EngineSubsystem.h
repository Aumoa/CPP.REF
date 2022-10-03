// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineSubsystem.gen.h"

SCLASS()
class GAME_API EngineSubsystem : implements Object
{
	GENERATED_BODY()

public:
	EngineSubsystem();

	virtual void Init();
	virtual void Deinit();
};