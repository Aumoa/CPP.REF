// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Text/TextBox.h"

GENERATE_BODY(STextBox);

STextBox::STextBox() : Super()
{
}

DEFINE_SLATE_CONSTRUCTOR(STextBox, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
}

bool STextBox::OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs)
{
	switch (EventArgs.Event)
	{
	case EIMEEvent::Char:
		ProcessChar(EventArgs.Char.ComposedChar);
		return true;
	}

	return false;
}

void STextBox::ProcessChar(wchar_t Ch)
{
	switch (Ch)
	{
	case '\b':
		Backspace();
		break;
	case '\r':
	case '\n':
		Enter();
		break;
	default:
		SetText(GetText() + Ch);
		break;
	}
}

void STextBox::Backspace()
{
	std::wstring Str = GetText();
	if (Str.length() > 0)
	{
		Str.erase(Str.end() - 1);
	}
	SetText(Str);
}

void STextBox::Enter()
{
	TextCommitted.Broadcast(GetText());
	SetText(L"");
}