// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SLeafWidget.h"

class GAME_API STextBlock : public SLeafWidget
{
	using This = STextBlock;
	using Super = SLeafWidget;

public:
	STextBlock();

	void SetText(String Text);
	//String GetText();
	//void SetFont(const SlateFont& Font);
	//SlateFont GetFont();
	//void SetTintColor(const Color& TintColor);
	//Color GetTintColor();
	//void SetTextAlignment(ERHITextAlignment Alignment) { SetTextAlignment_GameThread(Alignment); }
	//ERHITextAlignment GetTextAlignment();
	//void SetParagraphAlignment(ERHIParagraphAlignment Alignment) { SetParagraphAlignment_GameThread(Alignment); }
	//ERHIParagraphAlignment GetParagraphAlignment();

	virtual bool PrepassLayout() override;

protected:
	//virtual Vector2 ComputeDesiredSize() override;
	//virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

private:
	//void ReallocLayout();
	//void SetTextAlignment_GameThread(ERHITextAlignment Alignment);
	//void SetParagraphAlignment_GameThread(ERHIParagraphAlignment Alignment);

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(String, Text)
		//DECLARE_SLATE_ATTRIBUTE(SlateFont, Font)
		//DECLARE_SLATE_ATTRIBUTE(Color, TintColor, NamedColors::Black)
		//DECLARE_SLATE_ATTRIBUTE(ERHITextAlignment, TextAlignment, ERHITextAlignment::Leading)
		//DECLARE_SLATE_ATTRIBUTE(ERHIParagraphAlignment, ParagraphAlignment, ERHIParagraphAlignment::Near)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};