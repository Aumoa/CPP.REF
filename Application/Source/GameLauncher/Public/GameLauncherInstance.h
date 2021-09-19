// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance.h"

class SUserWidget;
class SLaunchConfig;

class GAMELAUNCHER_API SGameLauncherInstance : public SGameInstance
{
	GENERATED_BODY(SGameLauncherInstance)

private:
	std::vector<SUserWidget*> _widgets;
	SLaunchConfig* _config = nullptr;

public:
	virtual void Init() override;
	SLaunchConfig* GetConfig() const { return _config; }
};

extern SGameLauncherInstance* GGameInstance;