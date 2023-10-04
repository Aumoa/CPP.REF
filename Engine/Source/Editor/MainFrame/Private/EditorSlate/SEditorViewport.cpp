// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "EditorSlate/SEditorMenu.h"
#include "EditorSlate/SEditorInspectorView.h"
#include "EditorSlate/SEditorContentView.h"
#include "EditorSlate/SEditorHierarchyView.h"
#include "EditorSlate/SEditorPlayMenu.h"
#include "EditorSlate/SEditorGameViewport.h"
#include "Layout/SDockPanel.h"

SEditorViewport::SEditorViewport()
{
}

DEFINE_SLATE_CONSTRUCTOR(SEditorViewport, Args)
{
	NSlot SingleSlot = std::move(Args.SingleSlot);

	Args.SingleSlot = SEditorViewport::NSlot()
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
				SNew(SEditorGameViewport)
				+SEditorGameViewport::NSlot()
				.VAlignment(SingleSlot._VAlignment)
				.HAlignment(SingleSlot._HAlignment)
				.SlotPadding(SingleSlot._SlotPadding)
				[
					SingleSlot.Content
				]
			]
		];
}