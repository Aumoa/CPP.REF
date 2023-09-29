// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "EditorSlate/SEditorMenu.h"
#include "EditorSlate/SEditorInspectorView.h"
#include "EditorSlate/SEditorContentView.h"
#include "EditorSlate/SEditorHierarchyView.h"
#include "EditorSlate/SEditorPlayMenu.h"
#include "Layout/SCanvasPanel.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Layout/SVerticalBoxPanel.h"
#include "Layout/SSizeBox.h"
#include "Layout/SDockPanel.h"
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
		.VAlignment(EVerticalAlignment::Fill)
		.HAlignment(EHorizontalAlignment::Fill)
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
				SNew(SEditorInspectorView)
			]
			// Viewer
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Bottom)
			.HAlignment(EHorizontalAlignment::Fill)
			[
				SNew(SEditorContentView)
			]
			// Hierarchy
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Left)
			.VAlignment(EVerticalAlignment::Fill)
			[
				SNew(SEditorHierarchyView)
			]
			// GameMenu
			+SDockPanel::NSlot()
			.Dock(EDockAttach::Top)
			.HAlignment(EHorizontalAlignment::Fill)
			[
				SNew(SEditorPlayMenu)
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