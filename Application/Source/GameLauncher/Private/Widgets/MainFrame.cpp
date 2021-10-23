// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "LogGameLauncher.h"
#include "GameLauncherInstance.h"
#include "LaunchConfig.h"
#include "IFrameworkView.h"
#include "GameFramework/LocalPlayer.h"

SMainFrame::SMainFrame() : Super()
{
}

SWidget* SMainFrame::BuildSlateResources()
{
	_content = SNew(SHorizontalBoxPanel);

	for (auto& element : GGameInstance->GetConfig()->GetElements())
	{
		SBorder* content = MakeSlot(element.Icon, element.Name, element.Redirect);
		if (content)
		{
			std::move(_content->AddSlot())
				.SizeParam(ESizeRule::Auto, 1.0f)
				[
					content
				];
		}
	}

	return _content;
}

void SMainFrame::ReleaseSlateResources()
{
	if (_content)
	{
		DestroyObject(_content);
		_content = nullptr;
	}
}

SBorder* SMainFrame::MakeSlot(const std::filesystem::path& image, std::wstring_view name, const std::filesystem::path& redirect)
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

	auto view = SNew(SButton)
		[
			SNew(SCanvasPanel)
			+SCanvasPanel::Slot()
			.bAutoSize(true)
			[
				SNew(SBorder)
				.Padding(20.0f, 20.0f, 20.0f, 20.0f)
				[
					SNew(SImage)
					.Brush(texture->GetShaderResourceView(), Vector2(140.0f, 140.0f))
				]
			]
			+SCanvasPanel::Slot()
			.Offset(0.0f, 140.0f, 140.0f, 20.0f)
			[
				SNew(SCanvasPanel)
				+SCanvasPanel::Slot()
				.Anchors(0.5f)
				.Alignment(0.5f, 0.5f)
				.bAutoSize(true)
				[
					SNew(STextBlock)
					.Font(font->GetFontFace(), 11)
					.Text(name)
				]
			]
		];

	view->ButtonClicked.AddRaw([&, redirect = redirect.wstring()](EMouseButton button, EMouseButtonEvent event)
		{
			if (button == EMouseButton::Left && event == EMouseButtonEvent::Pressed)
			{
				ShellExecuteW((HWND)GGameInstance->GetLocalPlayer()->GetFrameworkView()->GetWindowHandle(), L"open", redirect.c_str(), nullptr, nullptr, SW_SHOW);
			}
		});

	return view;
}