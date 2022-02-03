// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LayoutEnums.h"

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