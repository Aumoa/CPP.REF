// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/ArrangedWidget.h"
#include "Layout/SlateVisibility.h"

class SLATECORE_API NArrangedChildrens
{
private:
	const ESlateVisibility::Enum VisibilityFilter;
	std::vector<NArrangedWidget> Widgets;

public:
	NArrangedChildrens(ESlateVisibility::Enum InVisibilityFilter);

	void AddWidget(ESlateVisibility::Enum VisibilityOverride, const NArrangedWidget& InArrangedWidget);
	void InsertWidget(ESlateVisibility::Enum VisibilityOverride, const NArrangedWidget& InArrangedWidget, size_t Index);
	bool Accepts(ESlateVisibility::Enum Visibility) const;

	ESlateVisibility::Enum GetVisibilityFilter() const;
	const std::vector<NArrangedWidget>& GetWidgets() const;
};