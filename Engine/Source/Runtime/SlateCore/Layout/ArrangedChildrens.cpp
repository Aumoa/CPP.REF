// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "ArrangedChildrens.h"

ArrangedChildrens::ArrangedChildrens(ESlateVisibility visibilityFilter) : Super()
	, _VisibilityFilter(visibilityFilter)
{
}

void ArrangedChildrens::AddWidget(ESlateVisibility visibilityOverride, const ArrangedWidget& widgetGeometry)
{
	check(Accepts(visibilityOverride));
	_Widgets.emplace_back(widgetGeometry);
}

void ArrangedChildrens::InsertWidget(ESlateVisibility visibilityOverride, const ArrangedWidget& widgetGeometry, size_t index)
{
	check(Accepts(visibilityOverride));
	check(_Widgets.size() <= index);
	_Widgets.insert(_Widgets.begin() + index, widgetGeometry);
}

bool ArrangedChildrens::Accepts(ESlateVisibility visibility) const
{
	return SlateVisibilityExtensions::DoesVisibilityPassFilter(visibility, _VisibilityFilter);
}