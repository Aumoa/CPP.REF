// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteTextLayout.h"
#include "D3D12Device.h"
#include "D3D12CommandQueue.h"

SDWriteTextLayout::SDWriteTextLayout(SDXGIFactory* factory, ComPtr<IDWriteTextLayout> layout) : Super(factory)
	, _layout(std::move(layout))
{
}

void SDWriteTextLayout::SetMaxSize(const Vector2& InSize)
{
	HR(_layout->SetMaxWidth(InSize.X));
	HR(_layout->SetMaxHeight(InSize.Y));
}

Vector2 SDWriteTextLayout::GetMaxSize()
{
	return Vector2(_layout->GetMaxWidth(), _layout->GetMaxHeight());
}

Vector2 SDWriteTextLayout::GetDesiredSize()
{
	DWRITE_TEXT_METRICS Metrics;
	HR(_layout->GetMetrics(&Metrics));
	return Vector2(Metrics.width, Metrics.height);
}