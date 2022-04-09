// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Asp
{
	class SOkObjectResult;

	class ASP_API SControllerBase : extends(SObject)
	{
		GENERATED_BODY(SControllerBase);

	protected:
		SControllerBase();

		SOkObjectResult* Ok(SObject* value);
	};
}