// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Text/STextBlock.h"
#include "STextBox.gen.h"

SCLASS()
class GAME_API STextBox : public STextBlock
{
	GENERATED_BODY()

public:
	STextBox();

public:
	BEGIN_SLATE_ATTRIBUTE
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State) override;
	virtual bool OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State) override;

public:
	DECLARE_MULTICAST_DELEGATE(TextCommittedDelegate, String);
	TextCommittedDelegate TextCommitted;

private:
	void ProcessChar(wchar_t Ch);
	void Composition(wchar_t Ch);
	void Backspace();
	void Enter();

private:
	String Text;
	std::optional<wchar_t> ComposingChar;
	size_t Caret = 0;
};