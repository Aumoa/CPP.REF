// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "EditorSlate/SEditorPlayMenu.h"
#include "Layout/SSizeBox.h"
#include "Layout/SHorizontalBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "GenericPlatform/GenericImage.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

DEFINE_SLATE_CONSTRUCTOR(SEditorPlayMenu, Args)
{
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
				.Brush(LoadTextureAsync(TEXT("Editor/MainFrame/PlayButton.png")), Vector2(70.0f, 70.0f))
			]
		]
	];
}

Task<std::shared_ptr<NRHITexture2D>> SEditorPlayMenu::LoadTextureAsync(String InFilename)
{
	auto Holder = SharedFromThis();
	String ImageName = Path::Combine(Environment::GetEngineDirectory(), TEXT("Content"), InFilename);
	auto Image = co_await NGenericImage::LoadFromFileAsync(ImageName);
	auto Texture = co_await NRHIGlobal::GetDynamicRHI().CreateTexture2DAsync(Image);
	co_return Texture;
}