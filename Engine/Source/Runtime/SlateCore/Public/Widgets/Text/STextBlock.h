// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"
#include "Draw/SlateFont.h"
#include "RHI/RHIEnums.h"

struct IRHITextLayout;
struct IRHITextFormat;
struct IRHISolidColorBrush;

class SLATECORE_API STextBlock : public SLeafWidget
{
	GENERATED_BODY(STextBlock)

private:
	class SRenderElement;

private:
	//SPROPERTY(Layout)
	//IRHITextLayout* Layout = nullptr;
	//SPROPERTY(Format)
	//IRHITextFormat* Format = nullptr;
	//SPROPERTY(TintBrush)
	//IRHISolidColorBrush* TintBrush = nullptr;

	SPROPERTY(CachedRenderElement)
	SRenderElement* CachedRenderElement = nullptr;

	std::wstring Text;
	SlateFont Font;
	Color TintColor = NamedColors::Black;
	ERHITextAlignment TextAlignment = ERHITextAlignment::Leading;
	ERHIParagraphAlignment ParagraphAlignment = ERHIParagraphAlignment::Near;

	Vector2 CachedLocalMaxSize = Vector2::Zero();
	Vector2 CachedDesiredSize = Vector2::Zero();

public:
	STextBlock();

	void SetText(std::wstring_view Text);
	std::wstring GetText();
	void SetFont(const SlateFont& Font);
	SlateFont GetFont();
	void SetTintColor(const Color& TintColor);
	Color GetTintColor();
	void SetTextAlignment(ERHITextAlignment Alignment) { SetTextAlignment_GameThread(Alignment); }
	ERHITextAlignment GetTextAlignment();
	void SetParagraphAlignment(ERHIParagraphAlignment Alignment) { SetParagraphAlignment_GameThread(Alignment); }
	ERHIParagraphAlignment GetParagraphAlignment();

	virtual bool PrepassLayout() override;

protected:
	virtual Vector2 ComputeDesiredSize() override;
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

private:
	void ReallocLayout();
	void SetTextAlignment_GameThread(ERHITextAlignment Alignment);
	void SetParagraphAlignment_GameThread(ERHIParagraphAlignment Alignment);

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