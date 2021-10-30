// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIncludes.h"
#include "WindowsBrush.h"

SWindowsBrush::SWindowsBrush(ID2D1Brush* InBrush) : Super()
	, BrushObject(InBrush)
{
}

void SWindowsBrush::SetOpacity(float InOpacity)
{
	BrushObject->SetOpacity(InOpacity);
}

float SWindowsBrush::GetOpacity()
{
	return BrushObject->GetOpacity();
}

void SWindowsBrush::SetTransform(const SlateRenderTransform& InTransform)
{
	BrushObject->SetTransform(GetRenderTransform(InTransform));
}

SlateRenderTransform SWindowsBrush::GetTransform()
{
	D2D1_MATRIX_3X2_F M;
	BrushObject->GetTransform(&M);
	return GetRenderTransform(M);
}