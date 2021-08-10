// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Layout/ArrangedWidget.h"
#include "Layout/LayoutImpl.h"
#include "Widgets/Widget.h"

using namespace std;

ArrangedWidget::ArrangedWidget(SWidget* widget, const Geometry& geometry)
	: _widget(widget)
	, _geometry(geometry)
{
}

wstring ArrangedWidget::ToString(wstring_view formatArgs) const
{
	return format(L"{} @ {}", _widget->ToString(formatArgs), _geometry.ToString(formatArgs));
}

bool ArrangedWidget::operator ==(const ArrangedWidget& rhs) const
{
	return _widget == rhs._widget;
}

bool ArrangedWidget::operator !=(const ArrangedWidget& rhs) const
{
	return _widget != rhs._widget;
}