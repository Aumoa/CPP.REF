// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SBoxPanel.h"

namespace libty::inline SlateCore
{
	class SLATECORE_API SVerticalBoxPanel : extends(SBoxPanel)
	{
		GENERATED_BODY(SVerticalBoxPanel);

	public:
		SVerticalBoxPanel();
		virtual ~SVerticalBoxPanel() override;
	};
}