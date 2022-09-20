// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/PlatformApplication.h"
#include "GameApplication.gen.h"

class Engine;
class SWindow;
class GameInstance;

SCLASS()
class GAME_API GameApplication : public PlatformApplication
{
	GENERATED_BODY()

private:
	SPROPERTY()
	IPlatformWindow* Window = nullptr;
	SPROPERTY()
	Engine* Engine = nullptr;
	SPROPERTY()
	GameInstance* GameInstance = nullptr;

protected:
	SubclassOf<::GameInstance> GameInstanceClass;

public:
	GameApplication();

	virtual int32 Startup(const CommandLineBuilder& builder) override;
	virtual void RequestExit(int32 exitCode) override;
	virtual void Tick() override;

private:
	PerformanceTimer FrameTimer;
	std::vector<std::shared_ptr<SWindow>> sWindows;

protected:
	virtual void OnApplicationShutdown() noexcept override;

	virtual Type* GetEngineType();
	virtual void InitializeSlateApplication(IPlatformWindow* initialWindow);
	virtual void InitializeGameFramework();
	virtual void FinalizeGameFramework();

private:
	void OnMainWindowDestroyed();
};