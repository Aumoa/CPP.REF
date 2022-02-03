// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Text/TextBox.h"

GENERATE_BODY(STextBox);

STextBox::STextBox() : Super()
{
}

DEFINE_SLATE_CONSTRUCTOR(STextBox, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	Text = GetText();
	Caret = Text.length();
}

bool STextBox::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
	if (Event == EKeyboardEvent::Pressed)
	{
		switch (Key)
		{
		case EKey::Left:
			if (Caret > 0)
			{
				Caret -= 1;
			}
			return true;
		case EKey::Right:
			if (Caret < Text.length())
			{
				Caret += 1;
			}
			return true;
		case EKey::Home:
			if (Caret != 0)
			{
				Caret = 0;
			}
			return true;
		case EKey::End:
			if (size_t Len = Text.length(); Caret != Len)
			{
				Caret = Len;
			}
			return true;
		}
	}

	return false;
}

bool STextBox::OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs)
{
	switch (EventArgs.Event)
	{
	case EIMEEvent::Char:
		ProcessChar(EventArgs.Char.ComposedChar);
		return true;
	case EIMEEvent::Composition:
		Composition(EventArgs.Composition.ComposingChar);
		return true;
	}

	return false;
}

void STextBox::ProcessChar(wchar_t Ch)
{
	ComposingChar.reset();

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
		Text.insert(Text.begin() + Caret, Ch);
		SetText(Text);
		Caret += 1;
		break;
	}
}

void STextBox::Composition(wchar_t Ch)
{
	if (Ch == L'\0')
	{
		ComposingChar.reset();
		SetText(Text);
	}
	else
	{
		ComposingChar = Ch;
		std::wstring Left = Text.substr(0, Caret);
		std::wstring Right = Text.substr(Caret);
		SetText(Left + Ch + Right);
	}
}

void STextBox::Backspace()
{
	if (Caret > 0)
	{
		Text.erase(Text.begin() + Caret - 1);
		Caret -= 1;
		SetText(Text);
	}
}

void STextBox::Enter()
{
	TextCommitted.Broadcast(GetText());
	SetText(L"");
	Text = L"";
	Caret = 0;
}