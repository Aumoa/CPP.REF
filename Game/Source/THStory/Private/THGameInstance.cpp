// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Levels/StartupLevel.h"
#include "Assets/StaticMesh.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "GameFramework/LocalPlayer.h"
#include "Widgets/Window.h"
#include "Widgets/Image/Image.h"
#include "Widgets/Panel/HorizontalBoxPanel.h"
#include "Widgets/Panel/VerticalBoxPanel.h"
#include "Widgets/Text/TextBlock.h"
#include "Assets/Font.h"

STHGameInstance::STHGameInstance() : Super()
{
	StartupLevel = SubclassOf<::SStartupLevel>::StaticClass();
}

void STHGameInstance::Init()
{
	Super::Init();

	auto* playerSystem = GEngine->GetEngineSubsystem<SGamePlayerSystem>();
	SLocalPlayer* localPlayer = playerSystem->GetLocalPlayer();
	SWindow* slateWindow = localPlayer->GetSlateWindow();

	SFont* font = LoadObject<SFont>(L"Engine/Content/Fonts/arial.ttf");

	auto* texture = LoadObject<STexture2D>(L"Game/Content/THStory/Art/SampleImage.jpg");
	slateWindow->AddWidgetToScreen(SNew(SHorizontalBoxPanel)
		+SHorizontalBoxPanel::Slot()
		.SizeParam(ESizeRule::Auto, 1.0f)
		[
			SNew(SImage)
			.Brush(texture->GetShaderResourceView(), Vector2(425.5f, 425.5f))
		]
		+SHorizontalBoxPanel::Slot()
		[
			SNew(SVerticalBoxPanel)
			+SVerticalBoxPanel::Slot()
			[
				SNew(STextBlock)
				.Text(L"FreeType font engine test!!")
				.Font(font->GetFontFace(), 70)
			]
			+SVerticalBoxPanel::Slot()
			[
				SNew(STextBlock)
				.Text(L"Text Advance Y Test")
				.Font(font->GetFontFace(), 60)
			]
			+SVerticalBoxPanel::Slot()
			[
				SNew(STextBlock)
				.Text(L"Font Kerning Text: AVAVAVAV")
				.Font(font->GetFontFace(), 80)
			]
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
