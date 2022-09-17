// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Engine.generated.h"

class GameRenderSubsystem;
class GameInstance;

SCLASS()
class GAME_API Engine : virtual public Object
{
	GENERATED_BODY()

private:
	SPROPERTY()
	GameRenderSubsystem* RenderSystem = nullptr;

protected:
	Engine();

public:
	virtual void Init();
	virtual void Deinit();
	virtual void Start(GameInstance* Instance);

	void ExecuteEngineLoop(const TimeSpan& deltaTime);

	template<std::same_as<GameRenderSubsystem> T>
	GameRenderSubsystem* GetEngineSubsystem() { return RenderSystem; }

protected:
	virtual void DispatchEngineTick(const TimeSpan& deltaTime);
};