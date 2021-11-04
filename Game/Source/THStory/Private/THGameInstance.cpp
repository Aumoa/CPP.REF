// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Level/StartupLevel.h"
#include "GameFramework/LocalPlayer.h"
#include "Widgets/Image/Image.h"
#include "Widgets/Panel/CanvasPanel.h"
#include "Application/Viewport.h"

STHGameInstance::STHGameInstance() : Super()
{
	StartupLevel = ::SStartupLevel::StaticClass();
}

void STHGameInstance::Init()
{
	Super::Init();

	SLocalPlayer* LocalPlayer = GetLocalPlayer();
	SViewport* GameViewport = LocalPlayer->GetGameViewport();

	SImage* RotatedImage;

	auto* Root = SNew(SCanvasPanel)
		+SCanvasPanel::Slot()
		.Offset(10.0f, 10.0f, 200.0f, 200.0f)
		[
			SNew(SImage)
			.Brush(NamedColors::Red)
		]
		+SCanvasPanel::Slot()
		.Anchors(0.5f, 0.5f)
		.Alignment(0.5f, 0.5f)
		.Offset(-120.0f, -120.0f, 200.0f, 200.0f)
		[
			SNew(SImage)
			.Brush(NamedColors::Red)
		]
		+SCanvasPanel::Slot()
		.Anchors(0.5f, 0.5f)
		.Alignment(0.5f, 0.5f)
		.Offset(+120.0f, -120.0f, 200.0f, 200.0f)
		[
			SNew(SImage)
			.Brush(NamedColors::Red)
		]
		+SCanvasPanel::Slot()
		.Anchors(0.5f, 0.5f)
		.Alignment(0.5f, 0.5f)
		.Offset(+120.0f, +120.0f, 200.0f, 200.0f)
		[
			SNew(SImage)
			.Brush(NamedColors::Red)
		]
		+SCanvasPanel::Slot()
		.Anchors(0.5f, 0.5f)
		.Alignment(0.5f, 0.5f)
		.Offset(-120.0f, +120.0f, 200.0f, 200.0f)
		[
			SAssignNew(RotatedImage, SImage)
			.Brush(NamedColors::Red)
		];

	RotatedImage->SetRenderTransform(Matrix2x2::Rotation(Degrees(45.0f).ToRadians()));

	GameViewport->AddToViewport(Root);
}