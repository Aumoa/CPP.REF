// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngine.h"

class SGameEngine;

class GAME_API SGameEngineSubsystem : implements SObject
{
	GENERATED_BODY(SGameEngineSubsystem)

public:
	SGameEngineSubsystem();
	virtual ~SGameEngineSubsystem() override;

	virtual void Init() {}
	virtual void Deinit() {}
	virtual void Tick(float InDeltaTime) {}

public:
	template<std::derived_from<SGameEngineSubsystem> T>
	static T* Get()
	{
		if (GEngine)
		{
			return GEngine->GetEngineSubsystem<T>();
		}
		return nullptr;
	}
};