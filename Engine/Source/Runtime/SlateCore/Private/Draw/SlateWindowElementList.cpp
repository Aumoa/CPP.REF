// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateWindowElementList.h"

SlateWindowElementList::SlateWindowElementList(SWindow* InPaintWindow)
	: PaintWindow(InPaintWindow)
{
}

void SlateWindowElementList::SortByLayer()
{
	std::sort(Elements.begin(), Elements.end(), [](const auto& Lhs, const auto& Rhs)
	{
		return Lhs.Layer < Rhs.Layer;
	});
}

SlateDrawElement& SlateWindowElementList::AddUninitialized()
{
	return Elements.emplace_back();
}

void SlateWindowElementList::Clear()
{
	Elements.clear();
}

const std::vector<SlateDrawElement>& SlateWindowElementList::GetElements() const
{
	return Elements;
}