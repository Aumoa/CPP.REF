// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIncludes.h"
#include "WindowsSolidColorBrush.h"

SWindowsSolidColorBrush::SWindowsSolidColorBrush(ID2D1SolidColorBrush* InBrushObject) : Super(InBrushObject)
	, SolidBrush(InBrushObject)
{
}

void SWindowsSolidColorBrush::SetColor(const Color& InColor)
{
	SolidBrush->SetColor(::GetColor(InColor));
}

Color SWindowsSolidColorBrush::GetColor()
{
	return ::GetColor(SolidBrush->GetColor());
}