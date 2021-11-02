// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LayoutImpl.h"
#include "ArrangedWidget.h"
#include "LayoutEnums.h"

class SLATECORE_API ArrangedChildrens
{
private:
	const ESlateVisibility VisibilityFilter;
	std::vector<ArrangedWidget> Widgets;

public:
	ArrangedChildrens(ESlateVisibility InVisibilityFilter);

	void AddWidget(ESlateVisibility VisibilityOverride, const ArrangedWidget& InArrangedWidget);
	void InsertWidget(ESlateVisibility VisibilityOverride, const ArrangedWidget& InArrangedWidget, size_t Index);
	bool Accepts(ESlateVisibility Visibility) const;

	ESlateVisibility GetVisibilityFilter() const;
	const std::vector<ArrangedWidget>& GetWidgets() const;
};