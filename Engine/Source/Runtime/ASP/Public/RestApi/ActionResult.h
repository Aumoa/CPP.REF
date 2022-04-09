// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IActionResult.h"

namespace libty::inline Asp
{
	class SActionResult : extends(SObject), implements(IActionResult)
	{
		GENERATED_BODY(SActionResult);

	protected:
		SActionResult();
	};
}