// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "LogGameLauncher.h"

SMainFrame::SMainFrame() : Super()
{
}

SWidget* SMainFrame::BuildSlateResources()
{
	auto texture = LoadObject<STexture2D>(L"Game/Content/GameLauncher/GenshinImpact_103.jpg");

	_content = SNew(SHorizontalBoxPanel)
		+SHorizontalBoxPanel::Slot()
		.SizeParam(ESizeRule::Auto, 1.0f)
		[
			MakeSlot(L"GenshinImpact_103.jpg", L"Genshin Impact")
		]
		+SHorizontalBoxPanel::Slot()
		.SizeParam(ESizeRule::Auto, 1.0f)
		[
			MakeSlot(L"GenshinImpact_103.jpg", L"Genshin Impact")
		];

	return _content;
}

void SMainFrame::ReleaseSlateResources()
{
	if (_content)
	{
		DestroySubobject(_content);
		_content = nullptr;
	}
}

SBorder* SMainFrame::MakeSlot(std::wstring_view image, std::wstring_view name)
{
	std::filesystem::path imagePath = std::filesystem::path(L"Game/Content/GameLauncher");
	imagePath /= image;
	auto texture = LoadObject<STexture2D>(imagePath);
	if (texture == nullptr)
	{
		SE_LOG(LogGameLauncher, Error, L"Image file({}) does not exists.", imagePath.wstring());
		return nullptr;
	}

	auto font = LoadObject<SFont>(L"Engine/Content/Fonts/arial.ttf");
	if (font == nullptr)
	{
		SE_LOG(LogGameLauncher, Error, L"Could not found engine font(Engine/Content/Fonts/arial.ttf).");
		return nullptr;
	}

	auto view = SNew(SBorder)
		.Padding(10.0f)
		[
			SNew(SCanvasPanel)
			+SCanvasPanel::Slot()
			.bAutoSize(true)
			[
				SNew(SImage)
				.Brush(texture->GetShaderResourceView(), Vector2(120.0f, 120.0f))
			]
			+SCanvasPanel::Slot()
			.Offset(0.0f, 120.0f, 120.0f, 20.0f)
			[
				SNew(STextBlock)
				.Font(font->GetFontFace(), 11)
				.Text(name)
			]
		];

	return view;
}