// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LeafWidget.h"
#include "Draw/SlateFont.h"

class SFontFace;

class GAME_API STextBlock : public SLeafWidget
{
	GENERATED_BODY(STextBlock)

private:
	std::wstring _text;
	SlateFont _font;

public:
	STextBlock(const std::wstring& name);
	virtual ~STextBlock() override;

	virtual Vector2 GetDesiredSize() override;

	void SetText(std::wstring_view text);
	std::wstring GetText();
	void SetFont(const SlateFont& font);
	SlateFont GetFont();

protected:
	virtual int32 OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(std::wstring, Text)
		DECLARE_SLATE_ATTRIBUTE(SlateFont, Font)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};