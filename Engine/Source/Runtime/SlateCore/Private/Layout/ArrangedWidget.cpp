// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Layout/ArrangedWidget.h"
#include "Widgets/SWidget.h"

using namespace ::libty;

ArrangedWidget::ArrangedWidget(SWidget* InWidget, const Geometry& InGeometry)
	: Widget(InWidget)
	, MyGeometry(InGeometry)
{
}

String ArrangedWidget::ToString() const
{
	return String::Format(L"{} @ {}", Widget->ToString(), MyGeometry.ToString());
}

bool ArrangedWidget::operator ==(const ArrangedWidget& Rhs) const
{
	return Widget == Rhs.Widget;
}

bool ArrangedWidget::operator !=(const ArrangedWidget& Rhs) const
{
	return Widget != Rhs.Widget;
}