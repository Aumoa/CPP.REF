// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Web/HttpStatusCode.h"

namespace libty::Asp::inline RestApi
{
	class ASP_API SMethodAttributeProducesResponseType : public SMethodAttribute
	{
		GENERATED_BODY(SMethodAttributeProducesResponseType);

	private:
		EHttpStatusCode _statusCode;

	public:
		SMethodAttributeProducesResponseType(EHttpStatusCode statusCode);

		EHttpStatusCode GetProducesCode();
	};
}