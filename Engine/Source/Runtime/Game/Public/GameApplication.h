// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/PlatformApplication.h"
#include "GameApplication.generated.h"

class Engine;

SCLASS()
class GAME_API GameApplication : public PlatformApplication
{
	GENERATED_BODY()

private:
	SPROPERTY()
	IPlatformWindow* _window = nullptr;
	SPROPERTY()
	Engine* _engine = nullptr;

public:
	SCONSTRUCTOR()
	GameApplication();

	virtual int32 Startup(const CommandLineBuilder& builder) override;
	virtual void Tick() override;

private:
	void OnMainWindowDestroyed();
};