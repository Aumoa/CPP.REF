// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Layout/ArrangedChildrens.h"

SArrangedChildrens::SArrangedChildrens(ESlateVisibility visibilityFilter) : Super()
	, _VisibilityFilter(visibilityFilter)
{
}

void SArrangedChildrens::AddWidget(ESlateVisibility visibilityOverride, const ArrangedWidget& widgetGeometry)
{
	check(Accepts(visibilityOverride));
	_Widgets.emplace_back(widgetGeometry);
}

void SArrangedChildrens::InsertWidget(ESlateVisibility visibilityOverride, const ArrangedWidget& widgetGeometry, size_t index)
{
	check(Accepts(visibilityOverride));
	check(_Widgets.size() <= index);
	_Widgets.insert(_Widgets.begin() + index, widgetGeometry);
}

bool SArrangedChildrens::Accepts(ESlateVisibility visibility) const
{
	return SlateVisibilityExtensions::DoesVisibilityPassFilter(visibility, _VisibilityFilter);
}