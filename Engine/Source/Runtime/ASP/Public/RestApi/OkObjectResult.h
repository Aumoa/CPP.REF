// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ObjectResult.h"

namespace libty::inline Asp
{
	class ASP_API SOkObjectResult : extends(SObjectResult)
	{
		GENERATED_BODY(SOkObjectResult);

	public:
		SOkObjectResult(SObject* value);
	};
}