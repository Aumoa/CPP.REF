// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IActionResult.h"

namespace libty::Asp::inline RestApi
{
	class SActionResult : virtual public SObject, implements(IActionResult)
	{
		GENERATED_BODY(SActionResult);

	protected:
		SActionResult();
	};
}