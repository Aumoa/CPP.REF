// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LeafWidget.h"
#include "Draw/SlateFont.h"

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

	Vector2 CachedLocalMaxSize = Vector2::ZeroVector();

public:
	STextBlock();

	void SetText(std::wstring_view Text);
	std::wstring GetText();
	void SetFont(const SlateFont& Font);
	SlateFont GetFont();
	void SetTintColor(const Color& TintColor);
	Color GetTintColor();

	virtual Vector2 GetDesiredSize() override;

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;

private:
	void ReallocLayout();

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(std::wstring, Text)
		DECLARE_SLATE_ATTRIBUTE(SlateFont, Font)
		DECLARE_SLATE_ATTRIBUTE(Color, TintColor)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};