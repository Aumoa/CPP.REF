// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Engine.generated.h"

class World;

SCLASS()
class GAME_API Engine : virtual public Object
{
	GENERATED_BODY()

private:
	PerformanceTimer _timer;

private:
	SPROPERTY()
	World* _world = nullptr;

protected:
	Engine();

public:
	virtual void Init();

	void ExecuteEngineLoop();

protected:
	virtual void DispatchEngineTick(float elapsedSeconds);

protected:
	World* SpawnGameWorld();
};