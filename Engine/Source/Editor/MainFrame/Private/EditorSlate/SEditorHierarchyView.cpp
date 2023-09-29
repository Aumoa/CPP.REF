// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorHierarchyView.h"
#include "Layout/SSizeBox.h"
#include "Layout/SVerticalBoxPanel.h"
#include "Layout/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"

DEFINE_SLATE_CONSTRUCTOR(SEditorHierarchyView, Args)
{
	Color BorderColor = Color::FromHtml(TEXT("#2A3441"));
	Color InnerColor = Color::FromHtml(TEXT("#445469"));

	Args.SingleSlot = SEditorHierarchyView::NSlot()
	.VAlignment(EVerticalAlignment::Fill)
	.HAlignment(EHorizontalAlignment::Fill)
	[
		SNew(SSizeBox)
		.OverrideWidth(300.0f)
		+SSizeBox::NSlot()
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
			.VAlignment(EVerticalAlignment::Fill)
			.HAlignment(EHorizontalAlignment::Fill)
			.SlotPadding(3.0f, 3.0f, 3.0f, 3.0f)
			[
				SNew(SVerticalBoxPanel)
				+SVerticalBoxPanel::NSlot()
				.SizeParam(ESizeRule::Auto, 1.0f)
				.HAlignment(EHorizontalAlignment::Center)
				.SlotPadding(0.0f, 10.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.FontFamilyName(TEXT("Arial"))
					.FontSize(15.0f)
					.Text(TEXT("Hierarchy"))
				]
			]
		]
	];
}