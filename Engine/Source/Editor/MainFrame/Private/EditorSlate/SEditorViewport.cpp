// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "Layout/SCanvasPanel.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Widgets/Images/SImage.h"

SEditorViewport::SEditorViewport()
{
}

DEFINE_SLATE_CONSTRUCTOR(SEditorViewport, Args)
{
	Args.Slots.emplace_back(SViewport::NSlot()
		[
			SNew(SCanvasPanel)
			+SCanvasPanel::NSlot()
			.Offset(100.0f, 100.0f, 100.0f, 100.0f)
			[
				SNew(SImage)
				.Brush(NSlateBrush{ .DrawSize = Vector2(100.0f, 100.0f) })
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
					.Brush(NSlateBrush{ .DrawSize = Vector2(100.0f, 100.0f) })
				]
				+SHorizontalBoxPanel::NSlot()
				.HAlignment(EHorizontalAlignment::Center)
				.VAlignment(EVerticalAlignment::Center)
				.SlotPadding(10.0f, 10.0f, 10.0f, 10.0f)
				[
					SNew(SImage)
					.Brush(NSlateBrush{ .DrawSize = Vector2(50.05, 50.0f) })
				]
			]
		]
	);
}