// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/Layout/SBorder.h"

SBorder::SBorder()
{
}

SBorder::~SBorder() noexcept
{
}

void SBorder::SetPadding(const NMargin& InPadding)
{
	if (Padding != InPadding)
	{
		Padding = InPadding;
		InvalidateLayoutAndVolatility();
	}
}

void SBorder::SetContent(std::shared_ptr<SWidget> InContent)
{
	if (Content != InContent)
	{
		Content = std::move(InContent);
		InvalidateLayoutAndVolatility();
	}
}

Vector2 SBorder::ComputeDesiredSize() const
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

void SBorder::OnArrangeChildren(NArrangedChildrens& ArrangedChildrens, const NGeometry& AllottedGeometry) const
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

DEFINE_SLATE_CONSTRUCTOR(SBorder, Args)
{
	Padding = Args._Padding;
	Content = Args._Content;
}