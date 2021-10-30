// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Text/TextBlock.h"
#include "Draw/SlateWindowElementList.h"

STextBlock::STextBlock() : Super()
{
}

STextBlock::~STextBlock()
{
}

Vector2 STextBlock::GetDesiredSize()
{
	//if (_font.FontFace)
	//{
	//	SFontCachingManager* cachingMgr = _font.FontFace->GetModule()->CreateCachingMgr(nullptr);
	//	if (cachingMgr)
	//	{
	//		Vector2 desiredSize;
	//		cachingMgr->StreamGlyphs(_font.FontFace, _text);
	//		std::vector<GlyphRenderInfo> glyphs = cachingMgr->QueryGlyphsRenderInfo(_font.FontFace, _font.FontSize, _text);

	//		if (glyphs.size())
	//		{
	//			for (auto& glyph : glyphs)
	//			{
	//				desiredSize.X += glyph.LocalAdvance.X;
	//				desiredSize.Y = std::max(desiredSize.Y, glyph.AbsoluteSize.Y);
	//			}

	//			desiredSize.X += glyphs.back().AbsoluteSize.X - glyphs.back().LocalAdvance.X;
	//			return desiredSize;
	//		}
	//	}
	//}

	return Vector2::ZeroVector();
}

void STextBlock::SetText(std::wstring_view text)
{
	_text = text;
}

std::wstring STextBlock::GetText()
{
	return _text;
}

void STextBlock::SetFont(const SlateFont& font)
{
	_font = font;
}

SlateFont STextBlock::GetFont()
{
	return _font;
}

int32 STextBlock::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	//SlateFontElement element;
	//element.FontFace = _font.FontFace;
	//element.FontSize = _font.FontSize;
	//element.Transform = allottedGeometry.ToPaintGeometry();
	//element.Layer = layer;
	//element.Text = _text;

	//drawElements->Add(element);
	return InLayer;
}

DEFINE_SLATE_CONSTRUCTOR(STextBlock, attr)
{
	Super::Construct(attr);
	SetText(attr._Text);
	SetFont(attr._Font);
}