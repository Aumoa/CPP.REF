// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SGameEngine;

class GAME_API SGameEngineSubsystem : virtual public SObject
{
	GENERATED_BODY(SGameEngineSubsystem)

public:
	SGameEngineSubsystem();
	virtual ~SGameEngineSubsystem() override;

	virtual void Init() {}
	virtual void Tick(std::chrono::duration<float> elapsedTime) {}
};