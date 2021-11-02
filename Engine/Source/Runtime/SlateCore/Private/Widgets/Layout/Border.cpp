// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Layout/Border.h"

SBorder::SBorder() : Super()
{
}

Vector2 SBorder::GetDesiredSize()
{
	if (Content && Content->GetVisibility() != ESlateVisibility::Collapsed)
	{
		return Content->GetDesiredSize() + Padding.GetTotalSpaceAlong();
	}
	else
	{
		return Vector2::ZeroVector();
	}
}

void SBorder::SetPadding(const Margin& InPadding)
{
	Padding = InPadding;
}

Margin SBorder::GetPadding()
{
	return Padding;
}

void SBorder::SetContent(SWidget* InContent)
{
	Content = InContent;
}

SWidget* SBorder::GetContent()
{
	return Content;
}

void SBorder::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	if (Content && Content->GetVisibility() != ESlateVisibility::Collapsed)
	{
		Vector2 Size = Content->GetDesiredSize();
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