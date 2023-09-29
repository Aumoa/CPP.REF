// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"
#include "Rendering/SlateBrush.h"

class NRHITextFormat;
class NRHITextLayout;
class NTextRenderAssetProxy;

class SLATECORE_API STextBlock : public SLeafWidget
{
	GENERATED_SLATE_BODY(STextBlock);

private:
	std::shared_ptr<NRHITextFormat> TextFormat;

	String CachedText;
	std::shared_ptr<NRHITextLayout> TextLayout;
	std::shared_ptr<NTextRenderAssetProxy> RenderProxy;

public:
	STextBlock();

	virtual void PrepassLayout() override;

	void SetText(String InText);
	String GetText() const { return CachedText; }

protected:
	virtual Vector2 ComputeDesiredSize() const override;
	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const override;

public:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_ATTRIBUTE(String, FontFamilyName)
		DECLARE_SLATE_ATTRIBUTE(float, FontSize)
		DECLARE_SLATE_ATTRIBUTE(bool, bBold)
		DECLARE_SLATE_ATTRIBUTE(bool, bItalic)
		DECLARE_SLATE_ATTRIBUTE(String, Text)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};