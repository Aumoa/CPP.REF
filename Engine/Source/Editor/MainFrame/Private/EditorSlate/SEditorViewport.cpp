// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "Layout/SCanvasPanel.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "GenericPlatform/GenericImage.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

SEditorViewport::SEditorViewport()
{
}

DEFINE_SLATE_CONSTRUCTOR(SEditorViewport, Args)
{
	auto ImageTask = LoadTextureAsync();

	Args.Slots.emplace_back(SViewport::NSlot()
		[
			SNew(SCanvasPanel)
			+SCanvasPanel::NSlot()
			.bAutoSize(true)
			.Offset(100.0f, 100.0f, 100.0f, 100.0f)
			.ZOrder(1)
			[
				SNew(SImage)
				.RenderOpacity(0.6f)
				.Brush(ImageTask, Vector2(571.0f, 200.0f))
			]
			+SCanvasPanel::NSlot()
			.bAutoSize(true)
			.Offset(100.0f, 220.0f)
			[
				SNew(SHorizontalBoxPanel)
				+SHorizontalBoxPanel::NSlot()
				.HAlignment(EHorizontalAlignment::Center)
				.VAlignment(EVerticalAlignment::Center)
				.SlotPadding(10.0f, 10.0f, 10.0f, 10.0f)
				[
					SNew(SImage)
					.Brush(ImageTask, Vector2(100.0f, 100.0f))
				]
				+SHorizontalBoxPanel::NSlot()
				.HAlignment(EHorizontalAlignment::Center)
				.VAlignment(EVerticalAlignment::Center)
				.SlotPadding(10.0f, 10.0f, 10.0f, 10.0f)
				[
					SNew(SImage)
					.Brush(ImageTask, Vector2(50.05, 50.0f))
				]
			]
		]
	);
}

Task<std::shared_ptr<NRHITexture2D>> SEditorViewport::LoadTextureAsync()
{
	auto Holder = SharedFromThis();
	String ImageName = Path::Combine(Environment::GetEngineDirectory(), TEXT("Content"), TEXT("Splash"), TEXT("SplashImage.png"));
	auto Image = co_await NGenericImage::LoadFromFileAsync(ImageName);
	auto Texture = co_await NRHIGlobal::GetDynamicRHI().CreateTexture2DAsync(Image);
	co_return Texture;
}