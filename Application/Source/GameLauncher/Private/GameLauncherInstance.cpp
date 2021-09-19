// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameLauncherInstance.h"
#include "Widgets/MainFrame.h"

SGameLauncherInstance* GGameInstance = nullptr;

void SGameLauncherInstance::Init()
{
	Super::Init();
	GGameInstance = this;

	NewObject<SMainFrame>()->AddWidgetToScreen();
}