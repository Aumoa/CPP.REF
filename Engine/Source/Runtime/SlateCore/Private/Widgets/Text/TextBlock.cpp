// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Text/TextBlock.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateDrawElement.h"
#include "RHI/IRHITextFormat.h"
#include "RHI/IRHITextLayout.h"
#include "RHI/IRHIFactory.h"
#include "IApplicationInterface.h"

STextBlock::STextBlock() : Super()
{
}

void STextBlock::SetText(std::wstring_view Text)
{
	this->Text = Text;
	bNeedToReallocateLayout = true;
}

std::wstring STextBlock::GetText()
{
	return Text;
}

void STextBlock::SetFont(const SlateFont& Font)
{
	this->Font = Font;
	bNeedToReallocateLayout = true;
}

SlateFont STextBlock::GetFont()
{
	return Font;
}

void STextBlock::SetTintColor(const Color& TintColor)
{
	this->TintColor = TintColor;
	bNeedToReallocateLayout = true;
}

Color STextBlock::GetTintColor()
{
	return TintColor;
}

Vector2 STextBlock::GetDesiredSize()
{
	if (Layout)
	{
		constexpr Vector2 SampleSize(1048576.0f, 1048576.0f);
		Layout->SetMaxSize(SampleSize);
		return Layout->GetDesiredSize();
	}
	else
	{
		return Vector2::ZeroVector();
	}
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

		Layout->SetOuter(this);
		bNeedToReallocateLayout = false;
	}
}

DEFINE_SLATE_CONSTRUCTOR(STextBlock, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	SetText(Attr._Text);
	SetFont(Attr._Font);
	SetTintColor(Attr._TintColor);
}