// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/LogConsole.h"
#include "Widgets/Image/Image.h"
#include "Widgets/Text/TextBlock.h"
#include "Diagnostics/LogModule.h"

GENERATE_BODY(SLogConsole);

SLogConsole::SLogConsole() : Super()
{
	LogModule::Get()->Logged.AddSObject(this, &SLogConsole::OnLogged);
}

SLogConsole::~SLogConsole() noexcept
{
	LogModule::Get()->Logged.RemoveAll(this);
}

Vector2 SLogConsole::ComputeDesiredSize()
{
	return Vector2(0.0f, 140.0f);
}

DEFINE_SLATE_CONSTRUCTOR(SLogConsole, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	AddSlot()
		.Offset(0.0f, 0.0f, 0.0f, 0.0f)
		.Anchors(0.0f, 0.0f, 1.0f, 1.0f)
		[
			SNew(SImage)
			.TintColor(NamedColors::DarkSlateGray)
		];

	AddSlot()
		.Offset(0.0f, 0.0f, 0.0f, 0.0f)
		.Anchors(0.0f, 0.0f, 1.0f, 1.0f)
		.ZOrder(1.0f)
		[
			SAssignNew(LogText, STextBlock)
			.TintColor(NamedColors::White)
			.Font(L"Consolas", 12.0f)
		];
}

void SLogConsole::OnLogged(std::wstring_view Message)
{
	if (Stream.size() >= 10)
	{
		Stream.erase(Stream.begin());
	}
	Stream.emplace_back(Message);

	LogText->SetText(StringUtils::Join(L"\n", Stream));
}