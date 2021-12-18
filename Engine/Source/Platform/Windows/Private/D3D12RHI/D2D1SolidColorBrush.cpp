// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1SolidColorBrush.h"

GENERATE_BODY(SD2D1SolidColorBrush);

SD2D1SolidColorBrush::SD2D1SolidColorBrush(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1SolidColorBrush* InBrush) : Super(InFactory, InDevice, InBrush)
	, Brush(InBrush)
{
}

void SD2D1SolidColorBrush::SetColor(const Color& InColor)
{
	Brush->SetColor((const D2D1_COLOR_F&)InColor);
}

Color SD2D1SolidColorBrush::GetColor()
{
	return (Color&)Brush->GetColor();
}