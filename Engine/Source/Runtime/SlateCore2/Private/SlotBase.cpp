// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlotBase.h"
#include "Widgets/SWidget.h"

using namespace ::libty;

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