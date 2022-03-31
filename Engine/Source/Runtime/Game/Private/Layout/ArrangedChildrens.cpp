// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Layout/ArrangedChildrens.h"

ArrangedChildrens::ArrangedChildrens(ESlateVisibility InVisibilityFilter)
	: VisibilityFilter(InVisibilityFilter)
{
}

void ArrangedChildrens::AddWidget(ESlateVisibility VisibiilityOverride, const ArrangedWidget& InArrangedWidget)
{
	checkf(Accepts(VisibiilityOverride), L"Cannot accept widget visibility. Check visibility with 'Accept' function first.");
	Widgets.emplace_back(InArrangedWidget);
}

void ArrangedChildrens::InsertWidget(ESlateVisibility VisibiilityOverride, const ArrangedWidget& InArrangedWidget, size_t Index)
{
	checkf(Accepts(VisibiilityOverride), L"Cannot accept widget visibility. Check visibility with 'Accept' function first.");
	checkf(Widgets.size() <= Index, L"Widget index overflow.");
	Widgets.insert(Widgets.begin() + Index, InArrangedWidget);
}

bool ArrangedChildrens::Accepts(ESlateVisibility visibility) const
{
	return SlateVisibilityExtensions::DoesVisibilityPassFilter(visibility, VisibilityFilter);
}

ESlateVisibility ArrangedChildrens::GetVisibilityFilter() const
{
	return VisibilityFilter;
}

const std::vector<ArrangedWidget>& ArrangedChildrens::GetWidgets() const
{
	return Widgets;
}