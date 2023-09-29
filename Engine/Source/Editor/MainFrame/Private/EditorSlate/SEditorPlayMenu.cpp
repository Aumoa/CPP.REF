// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorPlayMenu.h"
#include "Layout/SSizeBox.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "GenericPlatform/GenericImage.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "Assets/StreamableAssetManager.h"
#include "Assets/Texture2D.h"

DEFINE_SLATE_CONSTRUCTOR(SEditorPlayMenu, Args)
{
	auto Image = LoadObject(TEXT("/Engine/Editor/MainFrame/PlayButton.png"));

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
				.Brush(Image->GetRenderProxy(), Vector2(70.0f, 70.0f))
			]
		]
	];
}