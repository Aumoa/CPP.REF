// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LayoutEnums.h"

namespace libty::inline SlateCore
{
	struct SizeParam
	{
		ESizeRule SizeRule = ESizeRule::Stretch;
		float Value = 1.0f;

		constexpr SizeParam()
		{
		}

		constexpr SizeParam(ESizeRule SizeRule, float Value)
			: SizeRule(SizeRule)
			, Value(Value)
		{
		}
	};
}