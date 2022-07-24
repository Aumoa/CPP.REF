// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Engine.generated.h"

class World;
class GameRenderSubsystem;

SCLASS()
class GAME_API Engine : virtual public Object
{
	GENERATED_BODY()

private:
	PerformanceTimer _timer;

private:
	SPROPERTY()
	World* _world = nullptr;

	SPROPERTY()
	GameRenderSubsystem* _renderSystem = nullptr;

protected:
	Engine();

public:
	virtual void Init();
	virtual void Deinit();

	void ExecuteEngineLoop();

	template<class T>
	GameRenderSubsystem* GetEngineSubsystem() requires std::same_as<T, GameRenderSubsystem>
	{
		return _renderSystem;
	}

protected:
	virtual void DispatchEngineTick(float elapsedSeconds);

protected:
	World* SpawnGameWorld();
};