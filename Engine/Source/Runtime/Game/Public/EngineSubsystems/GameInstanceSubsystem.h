// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SGameInstance;

class GAME_API SGameInstanceSubsystem : implements SObject
{
	GENERATED_BODY(SGameInstanceSubsystem)

public:
	SGameInstanceSubsystem();
	
	virtual void Init() {}
	virtual void Deinit() {}

	SGameInstance* GetGameInstance();
};