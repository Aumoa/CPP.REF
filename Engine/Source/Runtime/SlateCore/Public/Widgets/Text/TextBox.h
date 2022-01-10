// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TextBlock.h"

class SLATECORE_API STextBox : public STextBlock
{
	GENERATED_BODY(STextBox)

public:
	STextBox();

public:
	BEGIN_SLATE_ATTRIBUTE
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual bool OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs) override;

public:
	DECLARE_MULTICAST_DELEGATE(TextCommittedDelegate, std::wstring_view)
	TextCommittedDelegate TextCommitted;

private:
	void ProcessChar(wchar_t Ch);
	void Backspace();
	void Enter();
};