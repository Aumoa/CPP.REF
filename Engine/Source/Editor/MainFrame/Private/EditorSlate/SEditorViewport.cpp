// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "EditorSlate/SEditorMenu.h"
#include "Layout/SCanvasPanel.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Layout/SVerticalBoxPanel.h"
#include "Layout/SSizeBox.h"
#include "Layout/SOverlay.h"
#include "Layout/SDockPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
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
			SNew(SDockPanel)

			// TOP MENU
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Top)
			.HAlignment(EHorizontalAlignment::Fill)
			[
				SNew(SEditorMenu)
			]
			// Inspector
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Right)
			.VAlignment(EVerticalAlignment::Fill)
			[
				SNew(SSizeBox)
				.OverrideWidth(250.0f)
				+SSizeBox::NSlot()
				.VAlignment(EVerticalAlignment::Fill)
				.HAlignment(EHorizontalAlignment::Fill)
				[
					SNew(SImage)
					.Brush(ImageTask)
				]
			]
			// Viewer
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Bottom)
			.HAlignment(EHorizontalAlignment::Fill)
			[
				SNew(SSizeBox)
				.OverrideHeight(300.0f)
				+SSizeBox::NSlot()
				.VAlignment(EVerticalAlignment::Fill)
				.HAlignment(EHorizontalAlignment::Fill)
				[
					SNew(SImage)
					.Brush(ImageTask)
				]
			]
			// Hierarchy
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Left)
			.VAlignment(EVerticalAlignment::Fill)
			[
				SNew(SSizeBox)
				.OverrideWidth(300.0f)
				+SSizeBox::NSlot()
				.VAlignment(EVerticalAlignment::Fill)
				.HAlignment(EHorizontalAlignment::Fill)
				[
					SNew(SImage)
					.Brush(ImageTask)
				]
			]
			// GameMenu
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Top)
			.HAlignment(EHorizontalAlignment::Fill)
			[
				SNew(SSizeBox)
				.OverrideHeight(80.0f)
				+SSizeBox::NSlot()
				.VAlignment(EVerticalAlignment::Fill)
				.HAlignment(EHorizontalAlignment::Fill)
				[
					SNew(SImage)
					.Brush(ImageTask)
				]
			]
			// GameViewport
			+SDockPanel::NSlot()
			.VAlignment(EVerticalAlignment::Fill)
			.HAlignment(EHorizontalAlignment::Fill)
			[
				SNew(SImage)
				.Brush(ImageTask)
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