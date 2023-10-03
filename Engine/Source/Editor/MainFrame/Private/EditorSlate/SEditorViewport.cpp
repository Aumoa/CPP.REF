// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "EditorSlate/SEditorMenu.h"
#include "EditorSlate/SEditorInspectorView.h"
#include "EditorSlate/SEditorContentView.h"
#include "EditorSlate/SEditorHierarchyView.h"
#include "EditorSlate/SEditorPlayMenu.h"
#include "Widgets/Images/SImage.h"
#include "Layout/SDockPanel.h"
#include "RenderGlobal.h"
#include "Assets/AssetsPath.h"

SEditorViewport::SEditorViewport()
{
}

DEFINE_SLATE_CONSTRUCTOR(SEditorViewport, Args)
{
	auto Image = NRenderGlobal::LoadTexture2DAsync(NAssetsPath::GetFileSystemPath(TEXT("/Engine/Splash/SplashImage.png")));

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
				.Brush(Image)
			]
		]
	);
}