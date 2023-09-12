// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorViewport.h"
#include "Layout/SCanvasPanel.h"
#include "Widgets/Images/SImage.h"

SEditorViewport::SEditorViewport()
{
}

void SEditorViewport::Tick([[maybe_unused]] const NGeometry& AllottedGeometry, [[maybe_unused]] const TimeSpan& InDeltaTime)
{
}

DEFINE_SLATE_CONSTRUCTOR(SEditorViewport, Args)
{
	Args.Slots.emplace_back(SViewport::NSlot()
		[
			SNew(SCanvasPanel)
			+ SCanvasPanel::NSlot()
			.bAutoSize(true)
			[
				SNew(SImage)
				.Brush(NSlateBrush{ .DrawSize = Vector2(500.0f, 500.0f) })
			]
		]
	);
}