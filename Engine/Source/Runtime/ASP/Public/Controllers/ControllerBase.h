// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::Asp::inline RestApi
{
	class SOkObjectResult;
}

namespace libty::Asp::inline Controllers
{
	class ASP_API SControllerBase : virtual public SObject
	{
		GENERATED_BODY(SControllerBase);

	protected:
		SControllerBase();

		SOkObjectResult* Ok(SObject* value);
	};
}