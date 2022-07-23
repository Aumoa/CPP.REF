// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Layout/ArrangedChildrens.h"

ArrangedChildrens::ArrangedChildrens(ESlateVisibility::Enum InVisibilityFilter)
	: VisibilityFilter(InVisibilityFilter)
{
}

void ArrangedChildrens::AddWidget(ESlateVisibility::Enum VisibiilityOverride, const ArrangedWidget& InArrangedWidget)
{
	checkf(Accepts(VisibiilityOverride), TEXT("Cannot accept widget visibility. Check visibility with 'Accept' function first."));
	Widgets.emplace_back(InArrangedWidget);
}

void ArrangedChildrens::InsertWidget(ESlateVisibility::Enum VisibiilityOverride, const ArrangedWidget& InArrangedWidget, size_t Index)
{
	checkf(Accepts(VisibiilityOverride), TEXT("Cannot accept widget visibility. Check visibility with 'Accept' function first."));
	checkf(Widgets.size() <= Index, TEXT("Widget index overflow."));
	Widgets.insert(Widgets.begin() + Index, InArrangedWidget);
}

bool ArrangedChildrens::Accepts(ESlateVisibility::Enum Visibility) const
{
	return ESlateVisibility::DoesVisibilityPassFilter(Visibility, VisibilityFilter);
}

ESlateVisibility::Enum ArrangedChildrens::GetVisibilityFilter() const
{
	return VisibilityFilter;
}

const std::vector<ArrangedWidget>& ArrangedChildrens::GetWidgets() const
{
	return Widgets;
}