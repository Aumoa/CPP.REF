// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Layout/ArrangedChildrens.h"

NArrangedChildrens::NArrangedChildrens(ESlateVisibility::Enum InVisibilityFilter)
	: VisibilityFilter(InVisibilityFilter)
{
}

void NArrangedChildrens::AddWidget(ESlateVisibility::Enum VisibiilityOverride, const NArrangedWidget& InArrangedWidget)
{
	checkf(Accepts(VisibiilityOverride), TEXT("Cannot accept widget visibility. Check visibility with 'Accept' function first."));
	Widgets.emplace_back(InArrangedWidget);
}

void NArrangedChildrens::InsertWidget(ESlateVisibility::Enum VisibiilityOverride, const NArrangedWidget& InArrangedWidget, size_t Index)
{
	checkf(Accepts(VisibiilityOverride), TEXT("Cannot accept widget visibility. Check visibility with 'Accept' function first."));
	checkf(Widgets.size() <= Index, TEXT("Widget index overflow."));
	Widgets.insert(Widgets.begin() + Index, InArrangedWidget);
}

bool NArrangedChildrens::Accepts(ESlateVisibility::Enum Visibility) const
{
	return ESlateVisibility::DoesVisibilityPassFilter(Visibility, VisibilityFilter);
}

ESlateVisibility::Enum NArrangedChildrens::GetVisibilityFilter() const
{
	return VisibilityFilter;
}

const std::vector<NArrangedWidget>& NArrangedChildrens::GetWidgets() const
{
	return Widgets;
}