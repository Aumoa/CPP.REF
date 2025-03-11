// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorPlayMenu.h"
#include "Widgets/Layout/SSizeBox.h"
#include "Widgets/Layout/SHorizontalBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "RenderGlobal.h"

DEFINE_SLATE_CONSTRUCTOR(SEditorPlayMenu, Args)
{
	//auto Image = NRenderGlobal::LoadTexture2DAsync(NAssetsPath::GetFileSystemPath(TEXT("/Engine/Editor/MainFrame/PlayButton.png")));

	Args.SingleSlot = SEditorPlayMenu::NSlot()
	.HAlignment(EHorizontalAlignment::Fill)
	.VAlignment(EVerticalAlignment::Fill)
	[
		SNew(SSizeBox)
		.OverrideHeight(80.0f)
		+SSizeBox::NSlot()
		.HAlignment(EHorizontalAlignment::Fill)
		.VAlignment(EVerticalAlignment::Fill)
		[
			SNew(SHorizontalBoxPanel)
			+SHorizontalBoxPanel::NSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			.SlotPadding(5.0f, 5.0f, 5.0f, 0.0f)
			[
				SNew(SImage)
				//.Brush(Image, Vector2(70.0f, 70.0f))
			]
		]
	];
}