// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Layout/ArrangedWidget.h"
#include "Widgets/SWidget.h"

NArrangedWidget::NArrangedWidget(std::shared_ptr<SWidget> InWidget, const NGeometry& InGeometry)
	: Widget(InWidget)
	, MyGeometry(InGeometry)
{
}

String NArrangedWidget::ToString() const
{
	return String::Format(TEXT("{} @ {}"), Widget->ToString(), MyGeometry.ToString());
}