// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateCore/Layout/ArrangedWidget.h"
#include "SlateCore/SWidget.h"

ArrangedWidget::ArrangedWidget(SWidget* InWidget, const Geometry& InGeometry)
	: Widget(InWidget)
	, MyGeometry(InGeometry)
{
}

std::wstring ArrangedWidget::ToString() const
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