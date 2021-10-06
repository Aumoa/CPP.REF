// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Levels/StartupLevel.h"
#include "Assets/StaticMesh.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "GameFramework/LocalPlayer.h"
#include "Widgets/SlateApplication.h"
#include "Widgets/Image/Image.h"
#include "Widgets/Panel/HorizontalBoxPanel.h"
#include "Widgets/Panel/VerticalBoxPanel.h"
#include "Widgets/Text/TextBlock.h"

STHGameInstance::STHGameInstance() : Super()
{
	StartupLevel = ::SStartupLevel::StaticClass();
}

void STHGameInstance::Init()
{
	Super::Init();

	auto* playerSystem = GEngine->GetEngineSubsystem<SGamePlayerSystem>();
	SLocalPlayer* localPlayer = playerSystem->GetLocalPlayer();
	SWindow* slateWindow = localPlayer->GetSlateWindow();
}