// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateCore/SlotBase.h"
#include "SlateCore/SWidget.h"

GENERATE_BODY(libty::SlateCore::Details::SSlotBase);

namespace libty::SlateCore::Details
{
	SSlotBase::SSlotBase()
	{
	}

	SWidget* SSlotBase::GetContent()
	{
		return Content;
	}

	void SSlotBase::SetContent(SWidget* Content)
	{
		this->Content = Content;
	}
}