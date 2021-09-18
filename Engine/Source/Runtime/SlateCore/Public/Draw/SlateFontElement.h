// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "FreeType/FontFaceCachingNode.h"
#include "Layout/PaintGeometry.h"

struct SlateGlyphElement
{
	Vector2 TexturePosition;
	Vector2 TextureSize;
};

struct SlateFontElement
{
	const SFontFaceCachingNode* CachingNode = nullptr;
	PaintGeometry Transform;
	int32 Layer = 0;
	std::vector<SlateGlyphElement> Glyphs;

	SlateFontElement()
	{
	}

	SlateFontElement(const SFontFaceCachingNode* cachingNode, const PaintGeometry& transform, int32 layer)
		: CachingNode(cachingNode)
		, Transform(transform)
		, Layer(layer)
	{
	}
};