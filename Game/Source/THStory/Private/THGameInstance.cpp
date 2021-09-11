// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Levels/StartupLevel.h"
#include "Assets/StaticMesh.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "GameFramework/LocalPlayer.h"
#include "Widgets/Window.h"
#include "Widgets/Images/Image.h"

THGameInstance::THGameInstance() : Super()
{
	StartupLevel = SubclassOf<::StartupLevel>::StaticClass();
}

void THGameInstance::Init()
{
	Super::Init();

	//auto* playerSystem = GEngine->GetEngineSubsystem<GamePlayerSystem>();
	//LocalPlayer* localPlayer = playerSystem->GetLocalPlayer();
	//SWindow* slateWindow = localPlayer->GetSlateWindow();

	//auto* imageWidget = NewObject<SImage>();
	//imageWidget->SetTexture2D(LoadObject<Texture2D>(L"Content/THStory/Art/SampleImage.jpg"));
	//slateWindow->AddWidgetToScreen(imageWidget);
}