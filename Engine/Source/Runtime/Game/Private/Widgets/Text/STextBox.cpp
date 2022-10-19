// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Input/STextBox.h"
#include "STextBox.gen.cpp"

STextBox::STextBox() : Super()
{
}

DEFINE_SLATE_CONSTRUCTOR(STextBox, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	//Text = GetText();
	Caret = (size_t)Text;
}

bool STextBox::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State)
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
			if (Caret < (size_t)Text)
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
			if (size_t Len = (size_t)Text; Caret != Len)
			{
				Caret = Len;
			}
			return true;
		}
	}

	return false;
}

bool STextBox::OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State)
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
		Text = Text.Insert(Caret, Ch);
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
		SetText(Text.Insert(Caret, Ch));
	}
}

void STextBox::Backspace()
{
	if (Caret > 0)
	{
		Text = Text.Remove(Caret - 1, 1);
		Caret -= 1;
		SetText(Text);
	}
}

void STextBox::Enter()
{
	//TextCommitted.Broadcast(GetText());
	SetText(TEXT(""));
	Text = TEXT("");
	Caret = 0;
}