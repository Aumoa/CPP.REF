// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SlotBase.h"

GENERATE_BODY(SWidgetSlot);

SWidgetSlot::SWidgetSlot() : Super()
{
}

void SWidgetSlot::SetContent(SWidget* InContent)
{
	checkf(Content == nullptr, L"Content already setted.");
	Content = InContent;
}

SWidget* SWidgetSlot::GetContent()
{
	return Content;
}