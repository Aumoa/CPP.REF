// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Text/TextBlock.h"
#include "Draw/SlateFontElement.h"
#include "Draw/SlateWindowElementList.h"
#include "FreeType/FontFace.h"
#include "FreeType/FontCachingManager.h"
#include "FreeType/FreeTypeModule.h"

STextBlock::STextBlock(const std::wstring& name) : Super(name)
{
}

STextBlock::~STextBlock()
{
}

Vector2 STextBlock::GetDesiredSize() const
{
	if (_font.FontFace)
	{
		SFontCachingManager* cachingMgr = _font.FontFace->GetModule()->CreateCachingMgr(nullptr);
		if (cachingMgr)
		{
			Vector2 desiredSize;
			cachingMgr->StreamGlyphs(_font.FontFace, _text);
			std::vector<GlyphRenderInfo> glyphs = cachingMgr->QueryGlyphsRenderInfo(_font.FontFace, _font.FontSize, _text);

			if (glyphs.size())
			{
				for (auto& glyph : glyphs)
				{
					desiredSize.X += glyph.LocalAdvance.X;
					desiredSize.Y = std::max(desiredSize.Y, glyph.AbsoluteSize.Y);
				}

				desiredSize.X += glyphs.back().AbsoluteSize.X - glyphs.back().LocalAdvance.X;
				return desiredSize;
			}
		}
	}

	return Vector2::GetZero();
}

void STextBlock::SetText(std::wstring_view text)
{
	_text = text;
}

std::wstring STextBlock::GetText() const
{
	return _text;
}

void STextBlock::SetFont(const SlateFont& font)
{
	_font = font;
}

SlateFont STextBlock::GetFont() const
{
	return _font;
}

int32 STextBlock::OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
	SlateFontElement element;
	element.FontFace = _font.FontFace;
	element.FontSize = _font.FontSize;
	element.Transform = allottedGeometry.ToPaintGeometry();
	element.Layer = layer;
	element.Text = _text;

	drawElements->Add(element);
	return layer;
}

DEFINE_SLATE_CONSTRUCTOR(STextBlock, attr)
{
	Super::Construct(attr);
	SetText(attr._Text);
	SetFont(attr._Font);
}