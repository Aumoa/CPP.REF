// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SizeParam.h"
#include "Layout/SizeRule.h"

struct NSizeParam
{
	ESizeRule SizeRule = ESizeRule::Stretch;
	float Value = 1.0f;

	constexpr NSizeParam()
	{
	}

	constexpr NSizeParam(ESizeRule SizeRule, float Value)
		: SizeRule(SizeRule)
		, Value(Value)
	{
	}
};