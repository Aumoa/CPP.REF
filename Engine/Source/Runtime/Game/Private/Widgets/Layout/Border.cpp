// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Layout/Border.h"
#include "Layout/Geometry.h"
#include "Layout/ArrangedChildrens.h"

SBorder::SBorder(const std::wstring& name) : Super(name)
{
}

Vector2 SBorder::GetDesiredSize()
{
	if (_content && _content->GetVisibility() != ESlateVisibility::Collapsed)
	{
		return _content->GetDesiredSize() + _padding.GetTotalSpaceAlong();
	}
	else
	{
		return Vector2::GetZero();
	}
}

void SBorder::SetPadding(const Margin& padding)
{
	_padding = padding;
}

Margin SBorder::GetPadding()
{
	return _padding;
}

void SBorder::SetContent(SWidget* content)
{
	_content = content;
}

SWidget* SBorder::GetContent()
{
	return _content;
}

void SBorder::OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry)
{
	if (_content && _content->GetVisibility() != ESlateVisibility::Collapsed)
	{
		Vector2 size = _content->GetDesiredSize();
		Vector2 paddingLT = Vector2(_padding.Left, _padding.Top);
		Vector2 paddingRB = Vector2(_padding.Right, _padding.Bottom);

		arrangedChildrens->AddWidget(_content->GetVisibility(), allottedGeometry.MakeChild(
			_content,
			paddingLT,
			size - paddingRB - paddingLT
		));
	}
}

DEFINE_SLATE_CONSTRUCTOR(SBorder, attr)
{
	Super::Construct(attr);

	_padding = attr._Padding;
	_content = attr._Content;
}