// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorMenu.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Layout/SSizeBox.h"
#include "Widgets/Text/STextBlock.h"

DEFINE_SLATE_CONSTRUCTOR(SEditorMenu, Args)
{
	Args.SingleSlot = SEditorMenu::NSlot()
	[
		SNew(SSizeBox)
		.OverrideHeight(20.0f)
		+SSizeBox::NSlot()
		.VAlignment(EVerticalAlignment::Fill)
		.HAlignment(EHorizontalAlignment::Fill)
		[
			SNew(SHorizontalBoxPanel)
			+SHorizontalBoxPanel::NSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			.SlotPadding(10.0f, 0.0f, 10.0f, 0.0f)
			[
				SNew(STextBlock)
				.FontFamilyName(TEXT("Arial"))
				.FontSize(15.0f)
				.Text(TEXT("프로젝트"))
			]
			+SHorizontalBoxPanel::NSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			.SlotPadding(10.0f, 0.0f, 10.0f, 0.0f)
			[
				SNew(STextBlock)
				.FontFamilyName(TEXT("Arial"))
				.FontSize(15.0f)
				.Text(TEXT("도구"))
			]
			+SHorizontalBoxPanel::NSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			.SlotPadding(10.0f, 0.0f, 10.0f, 0.0f)
			[
				SNew(STextBlock)
				.FontFamilyName(TEXT("Arial"))
				.FontSize(15.0f)
				.Text(TEXT("도움말"))
			]
		]
	];
}