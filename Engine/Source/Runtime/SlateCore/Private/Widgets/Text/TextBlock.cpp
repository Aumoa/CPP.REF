// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Text/TextBlock.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateDrawElement.h"
#include "RHI/IRHITextFormat.h"
#include "RHI/IRHITextLayout.h"
#include "RHI/IRHIFactory.h"
#include "IApplicationInterface.h"
#include "RenderThread.h"

STextBlock::STextBlock() : Super()
{
}

void STextBlock::SetText(std::wstring_view Text)
{
	if (this->Text != Text)
	{
		this->Text = Text;
		bNeedToReallocateLayout = true;
	}
}

std::wstring STextBlock::GetText()
{
	return Text;
}

void STextBlock::SetFont(const SlateFont& Font)
{
	if (this->Font != Font)
	{
		this->Font = Font;
		bNeedToReallocateLayout = true;
	}
}

SlateFont STextBlock::GetFont()
{
	return Font;
}

void STextBlock::SetTintColor(const Color& TintColor)
{
	this->TintColor = TintColor;
}

Color STextBlock::GetTintColor()
{
	return TintColor;
}

void STextBlock::SetTextAlignment(ERHITextAlignment Alignment)
{
	if (TextAlignment != Alignment)
	{
		TextAlignment = Alignment;

		if (Layout)
		{
			RenderThread::EnqueueRenderThreadWork<"STextBlock.SetTextAlignment">([&](auto)
			{
				Layout->SetTextAlignment(Alignment);
			});
		}
	}
}

ERHITextAlignment STextBlock::GetTextAlignment()
{
	return TextAlignment;
}

void STextBlock::SetParagraphAlignment(ERHIParagraphAlignment Alignment)
{
	if (ParagraphAlignment != Alignment)
	{
		ParagraphAlignment = Alignment;

		if (Layout)
		{
			RenderThread::EnqueueRenderThreadWork<"STextBlock.SetParagraphAlignment">([&](auto)
			{
				Layout->SetParagraphAlignment(Alignment);
			});
		}
	}
}

ERHIParagraphAlignment STextBlock::GetParagraphAlignment()
{
	return ParagraphAlignment;
}

Vector2 STextBlock::GetDesiredSize()
{
	return CachedDesiredSize;
}

int32 STextBlock::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	if (bNeedToReallocateLayout)
	{
		ReallocLayout();
	}

	if (Layout)
	{
		Vector2 LocalSize = AllottedGeometry.GetLocalSize();
		if (!CachedLocalMaxSize.NearlyEquals(LocalSize))
		{
			Layout->SetMaxSize(LocalSize);
			CachedLocalMaxSize = LocalSize;
		}
	}

	const PaintArgs NewArgs = Args.WithNewParent(this);
	SlateDrawElement::MakeText(InDrawElements, TintColor, Layout, NewArgs.RenderOpacity, AllottedGeometry.ToPaintGeometry(), InLayer);
	return InLayer;
}

void STextBlock::ReallocLayout()
{
	if (Layout)
	{
		DestroyObject(Layout);
		Layout = nullptr;
	}

	if (!Text.empty() && !Font.FamilyName.empty() && Font.Size > 0.1f)
	{
		IRHIFactory* Factory = IApplicationInterface::Get().GetFactory();
		IRHITextFormat* Format = Factory->CreateTextFormat(Font.FamilyName, Font.Collection, ERHIFontWeight::Normal, ERHIFontStyle::Normal, ERHIFontStretch::Normal, Font.Size, L"ko-KR");

		Vector2 LocalSize = Vector2(1048576.0f, 1048576.0f);
		Layout = Factory->CreateTextLayout(Format, Text, LocalSize);
		Format->SetOuter(Layout);
		CachedLocalMaxSize = LocalSize;
		CachedDesiredSize = Layout->GetDesiredSize();

		Layout->SetOuter(this);
		bNeedToReallocateLayout = false;

		Layout->SetTextAlignment(TextAlignment);
		Layout->SetParagraphAlignment(ParagraphAlignment);
	}
}

DEFINE_SLATE_CONSTRUCTOR(STextBlock, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	SetText(Attr._Text);
	SetFont(Attr._Font);
	SetTintColor(Attr._TintColor);
	SetTextAlignment(Attr._TextAlignment);
	SetParagraphAlignment(Attr._ParagraphAlignment);
	ReallocLayout();
}