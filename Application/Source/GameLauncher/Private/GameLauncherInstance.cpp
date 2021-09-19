// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameLauncherInstance.h"
#include "LaunchConfig.h"
#include "Widgets/MainFrame.h"

SGameLauncherInstance* GGameInstance = nullptr;

void SGameLauncherInstance::Init()
{
	Super::Init();
	GGameInstance = this;

	_config = NewObject<SLaunchConfig>();
	_config->Init();

	NewObject<SMainFrame>()->AddWidgetToScreen();
}