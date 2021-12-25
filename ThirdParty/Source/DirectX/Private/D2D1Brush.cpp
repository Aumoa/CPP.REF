// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1Brush.h"

GENERATE_BODY(SD2D1Brush);

SD2D1Brush::SD2D1Brush(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1Brush* InBrush) : Super(InFactory, InDevice, nullptr)
	, BrushObject(InBrush)
{
}

void SD2D1Brush::SetOpacity(float InValue)
{
	BrushObject->SetOpacity(InValue);
}

float SD2D1Brush::GetOpacity()
{
	return BrushObject->GetOpacity();
}