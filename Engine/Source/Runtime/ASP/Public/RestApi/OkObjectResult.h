// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ObjectResult.h"

namespace libty::Asp::inline RestApi
{
	class ASP_API SOkObjectResult : public SObjectResult
	{
		GENERATED_BODY(SOkObjectResult);

	public:
		SOkObjectResult(SObject* value);
	};
}