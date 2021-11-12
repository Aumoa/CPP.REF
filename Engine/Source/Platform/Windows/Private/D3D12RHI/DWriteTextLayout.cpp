// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteTextLayout.h"
#include "D3D12Device.h"
#include "D3D12CommandQueue.h"

SDWriteTextLayout::SDWriteTextLayout(SDXGIFactory* factory, ComPtr<IDWriteTextLayout> layout) : Super(factory)
	, Layout(std::move(layout))
{
}

void SDWriteTextLayout::SetMaxSize(const Vector2& InSize)
{
	HR(Layout->SetMaxWidth(InSize.X));
	HR(Layout->SetMaxHeight(InSize.Y));
}

Vector2 SDWriteTextLayout::GetMaxSize()
{
	return Vector2(Layout->GetMaxWidth(), Layout->GetMaxHeight());
}

Vector2 SDWriteTextLayout::GetDesiredSize()
{
	DWRITE_TEXT_METRICS Metrics;
	HR(Layout->GetMetrics(&Metrics));
	return Vector2(Metrics.width, Metrics.height);
}

void SDWriteTextLayout::SetTextAlignment(ERHITextAlignment Alignment)
{
	HR(Layout->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)Alignment));
}

ERHITextAlignment SDWriteTextLayout::GetTextAlignment()
{
	return (ERHITextAlignment)Layout->GetTextAlignment();
}

void SDWriteTextLayout::SetParagraphAlignment(ERHIParagraphAlignment Alignment)
{
	HR(Layout->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)Alignment));
}

ERHIParagraphAlignment SDWriteTextLayout::GetParagraphAlignment()
{
	return (ERHIParagraphAlignment)Layout->GetParagraphAlignment();
}