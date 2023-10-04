// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorGameViewport.h"
#include "Layout/SOverlay.h"
#include "Widgets/Images/SImage.h"

DEFINE_SLATE_CONSTRUCTOR(SEditorGameViewport, Args)
{
	Color BorderColor = Color::FromHtml(TEXT("#2A3441"));
	Color InnerColor = Color::FromHtml(TEXT("#445469"));

	Args.SingleSlot = SEditorGameViewport::NSlot()
	.VAlignment(EVerticalAlignment::Fill)
	.HAlignment(EHorizontalAlignment::Fill)
	[
		SNew(SOverlay)
		+SOverlay::NSlot()
		.VAlignment(EVerticalAlignment::Fill)
		.HAlignment(EHorizontalAlignment::Fill)
		[
			SNew(SImage)
			.Brush(Vector2(32.0f, 32.0f), BorderColor)
		]
		+SOverlay::NSlot()
		.VAlignment(EVerticalAlignment::Fill)
		.HAlignment(EHorizontalAlignment::Fill)
		.SlotPadding(3.0f, 3.0f, 3.0f, 3.0f)
		[
			SNew(SImage)
			.Brush(Vector2(32.0f, 32.0f), InnerColor)
		]
		+SOverlay::NSlot()
		.VAlignment(Args.SingleSlot._VAlignment)
		.HAlignment(Args.SingleSlot._HAlignment)
		.SlotPadding(Args.SingleSlot._SlotPadding + NMargin(3.0f, 3.0f, 3.0f, 3.0f))
		[
			Args.SingleSlot.Content
		]
	];
}