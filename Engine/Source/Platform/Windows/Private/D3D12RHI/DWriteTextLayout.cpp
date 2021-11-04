// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteTextLayout.h"
#include "D3D12Device.h"
#include "D3D12CommandQueue.h"

SDWriteTextLayout::SDWriteTextLayout(SDXGIFactory* factory, SD3D12Device* device, ComPtr<IDWriteTextLayout> layout) : Super(factory, device, nullptr)
	, _layout(std::move(layout))
{
}

Vector2 SDWriteTextLayout::GetMaxSize()
{
	return Vector2(_layout->GetMaxWidth(), _layout->GetMaxHeight());
}