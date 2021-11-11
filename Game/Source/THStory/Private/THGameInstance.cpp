// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Level/StartupLevel.h"
#include "GameFramework/LocalPlayer.h"
#include "Widgets/Image/Image.h"
#include "Widgets/Panel/CanvasPanel.h"
#include "Widgets/Panel/VerticalBoxPanel.h"
#include "Widgets/Text/TextBlock.h"
#include "Widgets/Input/Button.h"
#include "Application/Viewport.h"
#include "Animation/SlateAnimationContext.h"
#include "Animation/SlateAnimationPlayer.h"
#include "AnimCurves/CustomAnimationCurve.h"
#include "AnimCurves/LinearAnimationCurve.h"

STHGameInstance::STHGameInstance() : Super()
{
	StartupLevel = ::SStartupLevel::StaticClass();
}

void STHGameInstance::Init()
{
	Super::Init();

	SLocalPlayer* LocalPlayer = GetLocalPlayer();
	SViewport* GameViewport = LocalPlayer->GetGameViewport();

	SButton* Button;

	auto* Root = SNew(SCanvasPanel)
		//+SCanvasPanel::Slot()
		//.Offset(10.0f, 10.0f, 200.0f, 200.0f)
		//[
		//	SNew(SImage)
		//	.Brush(NamedColors::Red)
		//]
		+SCanvasPanel::Slot()
		.Offset(10.0f, 10.0f, 200.0f, 200.0f)
		.bAutoSize(true)
		[
			SNew(SVerticalBoxPanel)
			+SVerticalBoxPanel::Slot()
			[
				SNew(STextBlock)
				.Text(L"SampleText1")
				.Font(L"Arial", 30.0f)
				.TintColor(NamedColors::White)
			]
			+SVerticalBoxPanel::Slot()
			[
				SNew(STextBlock)
				.Text(L"SampleText2")
				.Font(L"Arial", 30.0f)
				.TintColor(NamedColors::White)
			]
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
			SAssignNew(Button, SButton)
			[
				SNew(SImage)
				.Brush(NamedColors::Red)
			]
		];

	static Degrees StartRotation = 0;

	auto* RotateAnim = NewObject<SSlateAnimationContext>(L"RotateAnim");
	RotateAnim->AddCurve(NewObject<SCustomAnimationCurve<SLinearAnimationCurve>>(0.0f, 45.0f, 0.0f, 0.5f, [Button](float Timing)
	{
		Degrees Rotation = StartRotation + Timing;
		Button->SetRenderTransform(Matrix2x2::Rotation(Rotation.ToRadians()));
	}));

	Button->GetAnimPlayer().AnimationFinished.AddRaw([Button, RotateAnim](std::wstring AnimName)
	{
		if (AnimName == RotateAnim->GetName())
		{
			StartRotation = (StartRotation + 45.0f).GetNormal();
			Button->PlayAnimation(RotateAnim);
		}
	});

	Button->PlayAnimation(RotateAnim);

	GameViewport->AddToViewport(Root);
}