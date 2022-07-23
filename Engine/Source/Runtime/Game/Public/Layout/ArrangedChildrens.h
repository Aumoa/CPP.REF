// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ArrangedWidget.h"
#include "LayoutEnums.h"

class GAME_API ArrangedChildrens
{
private:
	const ESlateVisibility::Enum VisibilityFilter;
	std::vector<ArrangedWidget> Widgets;

public:
	ArrangedChildrens(ESlateVisibility::Enum InVisibilityFilter);

	void AddWidget(ESlateVisibility::Enum VisibilityOverride, const ArrangedWidget& InArrangedWidget);
	void InsertWidget(ESlateVisibility::Enum VisibilityOverride, const ArrangedWidget& InArrangedWidget, size_t Index);
	bool Accepts(ESlateVisibility::Enum Visibility) const;

	ESlateVisibility::Enum GetVisibilityFilter() const;
	const std::vector<ArrangedWidget>& GetWidgets() const;
};