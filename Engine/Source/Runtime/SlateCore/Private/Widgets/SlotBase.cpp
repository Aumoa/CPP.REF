// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/SlotBase.h"

GENERATE_BODY(SWidgetSlot);

SWidgetSlot::SWidgetSlot() : Super()
{
}

void SWidgetSlot::SetContent(SWidget* InContent)
{
	check(Content == nullptr);
	Content = InContent;
}

SWidget* SWidgetSlot::GetContent()
{
	return Content;
}