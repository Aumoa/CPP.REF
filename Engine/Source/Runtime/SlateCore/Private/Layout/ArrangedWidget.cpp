// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Layout/ArrangedWidget.h"
#include "Layout/LayoutImpl.h"
#include "Widgets/Widget.h"

ArrangedWidget::ArrangedWidget(SWidget* InWidget, const Geometry& InGeometry)
	: Widget(InWidget)
	, MyGeometry(InGeometry)
{
}

std::wstring ArrangedWidget::ToString(std::wstring_view InFormatArgs) const
{
	return std::format(L"{} @ {}", Widget->ToString(InFormatArgs), MyGeometry.ToString(InFormatArgs));
}

bool ArrangedWidget::operator ==(const ArrangedWidget& Rhs) const
{
	return Widget == Rhs.Widget;
}

bool ArrangedWidget::operator !=(const ArrangedWidget& Rhs) const
{
	return Widget != Rhs.Widget;
}