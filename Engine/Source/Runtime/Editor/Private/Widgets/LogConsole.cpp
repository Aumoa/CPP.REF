// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/LogConsole.h"
#include "Widgets/Image/Image.h"
#include "Widgets/Text/TextBlock.h"
#include "Widgets/Text/TextBox.h"
#include "Widgets/Layout/ScrollBox.h"
#include "Widgets/Panel/VerticalBoxPanel.h"
#include "Diagnostics/LogModule.h"

GENERATE_BODY(SLogConsole);

DEFINE_LOG_CATEGORY(LogEditorConsole);

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
			SNew(SVerticalBoxPanel)
			+SVerticalBoxPanel::SSlot()
			.HAlignment(EHorizontalAlignment::Fill)
			.VAlignment(EVerticalAlignment::Fill)
			.SizeParam(ESizeRule::Stretch, 1.0f)
			[
				SAssignNew(ScrollBox, SScrollBox)
				+SScrollBox::SSlot()
				.HAlignment(EHorizontalAlignment::Fill)
				[
					SAssignNew(LogText, STextBlock)
					.TintColor(NamedColors::White)
					.Font(L"Consolas", 12.0f)
				]
			]
			+SVerticalBoxPanel::SSlot()
			.HAlignment(EHorizontalAlignment::Fill)
			.SizeParam(ESizeRule::Auto, 1.0f)
			[
				SAssignNew(ConsoleInput, STextBox)
				.TintColor(NamedColors::White)
				.Font(L"Consolas", 12.0f)
				.Text(L"여기에 입력")
			]
		];

	ConsoleInput->TextCommitted.AddSObject(this, &SLogConsole::OnConsoleCommitted);
}

void SLogConsole::OnLogged(std::wstring_view Message)
{
	Task<>::Deferred([&, Message = std::wstring(Message), Holder = WeakPtr(this)]()
	{
		if (Holder.IsValid())
		{
			if (!Stream.empty())
			{
				Stream += L"\n";
			}

			Stream += Message;
			LogText->SetText(Stream);

			ScrollBox->ScrollToBottom();
		}
	});
}

void SLogConsole::OnConsoleCommitted(std::wstring_view ConsoleInput)
{
	SE_LOG(LogEditorConsole, Verbose, L"ConsoleInput: {}", ConsoleInput);
}