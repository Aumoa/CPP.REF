// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LeafWidget.h"
#include "Draw/SlateFont.h"
#include "RHI/RHIEnums.h"

interface IRHITextLayout;

class SLATECORE_API STextBlock : public SLeafWidget
{
	GENERATED_BODY(STextBlock)

private:
	IRHITextLayout* Layout = nullptr;
	std::wstring Text;
	SlateFont Font;
	Color TintColor = NamedColors::Black;
	bool bNeedToReallocateLayout = false;
	ERHITextAlignment TextAlignment = ERHITextAlignment::Leading;
	ERHIParagraphAlignment ParagraphAlignment = ERHIParagraphAlignment::Near;

	Vector2 CachedLocalMaxSize = Vector2::ZeroVector();

public:
	STextBlock();

	void SetText(std::wstring_view Text);
	std::wstring GetText();
	void SetFont(const SlateFont& Font);
	SlateFont GetFont();
	void SetTintColor(const Color& TintColor);
	Color GetTintColor();
	void SetTextAlignment(ERHITextAlignment Alignment);
	ERHITextAlignment GetTextAlignment();
	void SetParagraphAlignment(ERHIParagraphAlignment Alignment);
	ERHIParagraphAlignment GetParagraphAlignment();

	virtual Vector2 GetDesiredSize() override;

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;

private:
	void ReallocLayout();

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(std::wstring, Text)
		DECLARE_SLATE_ATTRIBUTE(SlateFont, Font)
		DECLARE_SLATE_ATTRIBUTE(Color, TintColor, NamedColors::Black)
		DECLARE_SLATE_ATTRIBUTE(ERHITextAlignment, TextAlignment, ERHITextAlignment::Leading)
		DECLARE_SLATE_ATTRIBUTE(ERHIParagraphAlignment, ParagraphAlignment, ERHIParagraphAlignment::Near)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};