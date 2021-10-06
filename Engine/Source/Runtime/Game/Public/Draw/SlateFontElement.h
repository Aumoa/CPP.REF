// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
//#include "FreeType/FontFaceCachingNode.h"
#include "Layout/PaintGeometry.h"

class SFontFace;

struct SlateFontElement
{
	SFontFace* FontFace = nullptr;
	int32 FontSize = 0;
	PaintGeometry Transform;
	int32 Layer = 0;
	std::wstring Text;

	SlateFontElement()
	{
	}

	SlateFontElement(SFontFace* fontFace, const PaintGeometry& transform, int32 layer)
		: FontFace(fontFace)
		, Transform(transform)
		, Layer(layer)
	{
	}
};