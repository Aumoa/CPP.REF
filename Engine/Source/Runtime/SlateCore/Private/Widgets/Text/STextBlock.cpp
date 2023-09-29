// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/Text/STextBlock.h"
#include "Rendering/SlateTextFormatCache.h"
#include "Rendering/TextRenderProxy.h"
#include "RHI/RHITextLayout.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

STextBlock::STextBlock()
{
}

void STextBlock::PrepassLayout()
{
	String LayoutText = TextLayout ? TextLayout->GetText() : String::GetEmpty();
	if (LayoutText != CachedText)
	{
		TextLayout = NRHIGlobal::GetDynamicRHI().CreateTextLayout(TextFormat, CachedText);
		RenderProxy = std::make_shared<NTextRenderProxy>(TextLayout);
	}
	Super::PrepassLayout();
}

void STextBlock::SetText(String InText)
{
	if (CachedText != InText)
	{
		CachedText = InText;
		InvalidateLayoutAndVolatility();
	}
}

Vector2 STextBlock::ComputeDesiredSize() const
{
	if (TextLayout)
	{
		return TextLayout->GetDrawingSize();
	}
	return Vector2::Zero();
}

int32 STextBlock::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	PLATFORM_UNREFERENCED_PARAMETER(CullingRect);
	PLATFORM_UNREFERENCED_PARAMETER(bParentEnabled);

	NSlateRenderElement& Element = OutDrawElements.Add();
	Element.AbsolutePosition = AllottedGeometry.GetAbsolutePosition();
	Element.Layout = AllottedGeometry.ToPaintGeometry();

	Element.Layer = InLayer;
	Element.TintColor = 1.0f;
	Element.RenderOpacity = Args.RenderOpacity;

	Element.Proxy = RenderProxy;
	return InLayer;
}

DEFINE_SLATE_CONSTRUCTOR(STextBlock, Args)
{
	TextFormat = NSlateTextFormatCache::GetCachedTextFormat(Args._FontFamilyName, Args._FontSize, Args._bBold, Args._bItalic);
	CachedText = Args._Text;
}