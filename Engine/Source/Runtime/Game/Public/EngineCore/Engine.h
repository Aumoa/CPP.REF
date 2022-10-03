// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Engine.gen.h"

class EngineSubsystem;
class GameRenderSubsystem;
class GameInstance;

SCLASS()
class GAME_API Engine : implements Object
{
	GENERATED_BODY()

private:
	SPROPERTY()
	std::vector<EngineSubsystem*> Subsystems;
	std::map<Type*, EngineSubsystem*> CachedSubsystemMap;

protected:
	Engine();

public:
	virtual void Init();
	virtual void Deinit();
	virtual void Start(GameInstance* Instance);

	void ExecuteEngineLoop(const TimeSpan& deltaTime);

	template<std::derived_from<EngineSubsystem> T>
	T* GetEngineSubsystem() { return Cast<T>(GetEngineSubsystem(typeof(T))); }
	EngineSubsystem* GetEngineSubsystem(Type* SubsystemType);

protected:
	virtual void DispatchEngineTick(const TimeSpan& deltaTime);
};