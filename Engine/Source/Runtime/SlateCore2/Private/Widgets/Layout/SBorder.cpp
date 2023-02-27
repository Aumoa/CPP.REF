// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Layout/SBorder.h"

using namespace ::libty;

SBorder::SBorder() : Super()
{
}

void SBorder::SetPadding(const Margin& InPadding)
{
	if (Padding != InPadding)
	{
		Padding = InPadding;
		InvalidateLayoutAndVolatility();
	}
}

Margin SBorder::GetPadding()
{
	return Padding;
}

void SBorder::SetContent(SWidget* InContent)
{
	if (Content != InContent)
	{
		Content = InContent;
		InvalidateLayoutAndVolatility();
	}
}

SWidget* SBorder::GetContent()
{
	return Content;
}

Vector2 SBorder::ComputeDesiredSize()
{
	if (Content && Content->GetVisibility() != ESlateVisibility::Collapsed)
	{
		return Content->GetDesiredSize() + Padding.GetTotalSpaceAlong();
	}
	else
	{
		return Vector2::Zero();
	}
}

void SBorder::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	if (Content && Content->GetVisibility() != ESlateVisibility::Collapsed)
	{
		Vector2 Size = AllottedGeometry.GetLocalSize();
		Vector2 PaddingLT = Vector2(Padding.Left, Padding.Top);
		Vector2 PaddingRB = Vector2(Padding.Right, Padding.Bottom);

		ArrangedChildrens.AddWidget(Content->GetVisibility(), AllottedGeometry.MakeChild(
			Content,
			PaddingLT,
			Size - PaddingRB - PaddingLT
		));
	}
}

DEFINE_SLATE_CONSTRUCTOR(SBorder, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	Padding = Attr._Padding;
	Content = Attr._Content;
}