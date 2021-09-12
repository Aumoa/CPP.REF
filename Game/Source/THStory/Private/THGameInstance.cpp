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
#include "Widgets/Panel/HorizontalBoxPanel.h"

THGameInstance::THGameInstance() : Super()
{
	StartupLevel = SubclassOf<::StartupLevel>::StaticClass();
}

void THGameInstance::Init()
{
	Super::Init();

	auto* playerSystem = GEngine->GetEngineSubsystem<GamePlayerSystem>();
	LocalPlayer* localPlayer = playerSystem->GetLocalPlayer();
	SWindow* slateWindow = localPlayer->GetSlateWindow();

	auto* texture = LoadObject<Texture2D>(L"Content/THStory/Art/SampleImage.jpg");
	slateWindow->AddWidgetToScreen(SNew(SHorizontalBoxPanel)
		+SHorizontalBoxPanel::Slot()
		.SizeParam(ESizeRule::Auto, 1.0f)
		[
			SNew(SImage)
			.Brush(texture)
		]
	);

	//slateWindow->AddWidgetToScreen(SNew(SCanvasPanel)
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 0.0f, 100.0f, 100.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image100")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 0.0f, 100.0f, 100.0f)
	//	.Anchors(0.5f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image200")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 0.0f, 100.0f, 100.0f)
	//	.Anchors(1.0f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image300")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 100.0f, 100.0f, 100.0f)
	//	.Alignment(0.5f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image010")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 100.0f, 100.0f, 100.0f)
	//	.Anchors(0.5f, 0.0f)
	//	.Alignment(0.5f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image020")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 100.0f, 100.0f, 100.0f)
	//	.Anchors(1.0f, 0.0f)
	//	.Alignment(0.5f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image030")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 200.0f, 100.0f, 100.0f)
	//	.Alignment(1.0f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image001")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 200.0f, 100.0f, 100.0f)
	//	.Anchors(0.5f, 0.0f)
	//	.Alignment(1.0f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image002")
	//		.Brush(texture)
	//	]
	//	+SCanvasPanel::Slot()
	//	.Offset(0.0f, 200.0f, 100.0f, 100.0f)
	//	.Anchors(1.0f, 0.0f)
	//	.Alignment(1.0f, 0.0f)
	//	[
	//		SNew(SImage)
	//		.Name(L"Image003")
	//		.Brush(texture)
	//	]);
}
