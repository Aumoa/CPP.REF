// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "LayoutImpl.h"
#include "ArrangedWidget.h"
#include "Draw/SlateVisibility.h"

class ArrangedChildrens : virtual public Object
{
public:
	using Super = Object;

private:
	const ESlateVisibility _VisibilityFilter;
	std::vector<ArrangedWidget> _Widgets;

public:
	ArrangedChildrens(ESlateVisibility visibilityFilter);

	void AddWidget(ESlateVisibility visibilityOverride, const ArrangedWidget& widgetGeometry);
	void InsertWidget(ESlateVisibility visibilityOverride, const ArrangedWidget& widgetGeometry, size_t index);
	bool Accepts(ESlateVisibility visibility) const;

	ESlateVisibility GetVisibilityFilter() const { return _VisibilityFilter; }
	std::span<ArrangedWidget const> GetWidgets() const { return _Widgets; }
};