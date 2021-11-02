// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Layout/ArrangedChildrens.h"

ArrangedChildrens::ArrangedChildrens(ESlateVisibility InVisibilityFilter)
	: VisibilityFilter(InVisibilityFilter)
{
}

void ArrangedChildrens::AddWidget(ESlateVisibility VisibiilityOverride, const ArrangedWidget& InArrangedWidget)
{
	check(Accepts(VisibiilityOverride));
	Widgets.emplace_back(InArrangedWidget);
}

void ArrangedChildrens::InsertWidget(ESlateVisibility VisibiilityOverride, const ArrangedWidget& InArrangedWidget, size_t Index)
{
	check(Accepts(VisibiilityOverride));
	check(Widgets.size() <= Index);
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