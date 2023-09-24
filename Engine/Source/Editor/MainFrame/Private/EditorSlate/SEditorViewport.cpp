// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "Layout/SCanvasPanel.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Layout/SVerticalBoxPanel.h"
#include "Layout/SSizeBox.h"
#include "Layout/SOverlay.h"
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
			SNew(SVerticalBoxPanel)

			// TOP MENU
			+SVerticalBoxPanel::NSlot()
			.HAlignment(EHorizontalAlignment::Fill)
			[
				SNew(SSizeBox)
				.OverrideHeight(30.0f)
				+SSizeBox::NSlot()
				.VAlignment(EVerticalAlignment::Fill)
				.HAlignment(EHorizontalAlignment::Fill)
				[
					SNew(SOverlay)
					+SOverlay::NSlot()
					.HAlignment(EHorizontalAlignment::Fill)
					[
						SNew(SImage)
						.Brush(ImageTask)
					]
					+SOverlay::NSlot()
					[
						SNew(SHorizontalBoxPanel)
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
						+SHorizontalBoxPanel::NSlot()
						[
							SNew(SImage)
							.Brush(ImageTask, Vector2(100.0f))
						]
					]
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